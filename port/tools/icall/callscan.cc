// GCC plugin: every call's ABI-relevant view after the "optimized" pass
#include "gcc-plugin.h"
#include "plugin-version.h"
#include "tree.h"
#include "tree-pass.h"
#include "context.h"
#include "function.h"
#include "basic-block.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "tree-pretty-print.h"
#include "ssa.h"
#include "tree-dfa.h"
#include "stringpool.h"
#include "attribs.h"
#include <stdio.h>
#include <string>
int plugin_is_GPL_compatible;
static FILE *out, *pout;
#include <vector>
static int probe;
static tree probe_decl;
static unsigned fnv(const char *a, unsigned h) { for (; *a; a++) h = (h ^ (unsigned char)*a) * 16777619u; return h; }
static std::string tcls(tree t);
// the scalar fields of a small aggregate, flattened: "i2@0,i2@2,f4@4" (bitfields: "b@<byte>")
static void flat(tree t, long base, std::string &o, int depth) {
  if (depth > 6 || o.size() > 1200) { o += "?"; return; }
  if (TREE_CODE(t) == RECORD_TYPE || TREE_CODE(t) == UNION_TYPE || TREE_CODE(t) == QUAL_UNION_TYPE) {
    for (tree f = TYPE_FIELDS(t); f; f = DECL_CHAIN(f)) {
      if (TREE_CODE(f) != FIELD_DECL) continue;
      if (!tree_fits_shwi_p(bit_position(f))) { o += "?"; return; }
      long bp = tree_to_shwi(bit_position(f));
      if (DECL_BIT_FIELD(f)) { char b[32]; snprintf(b, sizeof b, "%sb@%ld", o.empty() ? "" : ",", base + bp / 8); o += b; continue; }
      flat(TREE_TYPE(f), base + bp / 8, o, depth + 1);
      if (TREE_CODE(t) != RECORD_TYPE) o += "|";   // a union member alternative
    }
    return;
  }
  if (TREE_CODE(t) == ARRAY_TYPE && TYPE_SIZE_UNIT(t) && tree_fits_uhwi_p(TYPE_SIZE_UNIT(t))) {
    tree e = TREE_TYPE(t);
    if (!TYPE_SIZE_UNIT(e) || !tree_fits_uhwi_p(TYPE_SIZE_UNIT(e)) || !tree_to_uhwi(TYPE_SIZE_UNIT(e))) { o += "?"; return; }
    long es = tree_to_uhwi(TYPE_SIZE_UNIT(e)), n = tree_to_uhwi(TYPE_SIZE_UNIT(t)) / es;
    for (long i = 0; i < n && i < 64; i++) flat(e, base + i * es, o, depth + 1);
    return;
  }
  char b[48]; snprintf(b, sizeof b, "%s%s@%ld", o.empty() || o.back() == '|' ? "" : ",", tcls(t).c_str(), base); o += b;
}
static std::string tcls(tree t) {
  if (!t) return "?";
  char b[64];
  switch (TREE_CODE(t)) {
  case VOID_TYPE: return "v";
  case POINTER_TYPE: case REFERENCE_TYPE: case OFFSET_TYPE: case NULLPTR_TYPE: return "p";
  case REAL_TYPE: snprintf(b, sizeof b, "f%d", (int)tree_to_uhwi(TYPE_SIZE_UNIT(t))); return b;
  case RECORD_TYPE: case UNION_TYPE: case QUAL_UNION_TYPE: case ARRAY_TYPE: case COMPLEX_TYPE: case VECTOR_TYPE:
    if (TYPE_SIZE_UNIT(t) && tree_fits_uhwi_p(TYPE_SIZE_UNIT(t))) {
      long sz = tree_to_uhwi(TYPE_SIZE_UNIT(t));
      snprintf(b, sizeof b, "%s%ld", TREE_CODE(t) == UNION_TYPE ? (TYPE_TRANSPARENT_AGGR(t) ? "t" : "u") : (TREE_CODE(t) == RECORD_TYPE ? "s" : "a"), sz);
      std::string r = b;
      if (sz <= 128 && TREE_CODE(t) != ARRAY_TYPE) { std::string o; flat(t, 0, o, 0); r += "{" + o + "}"; }
      return r;
    }
    return "s?";
  default:
    if (INTEGRAL_TYPE_P(t)) { snprintf(b, sizeof b, "i%d", (int)tree_to_uhwi(TYPE_SIZE_UNIT(t))); return b; }
    return std::string("?") + get_tree_code_name(TREE_CODE(t));
  }
}
static std::string tname(tree t) {
  if (!t) return "?";
  pretty_printer pp;
  dump_generic_node(&pp, t, 0, TDF_NONE, false);
  std::string s = pp_formatted_text(&pp);
  for (auto &c : s) if (c == '\t' || c == '\n') c = ' ';
  return s;
}
static std::string expr(tree t) { return tname(t); }
static std::string where(tree fn) {  // how the callee pointer was computed (a couple of defs back)
  std::string s;
  for (int k = 0; k < 3 && fn && TREE_CODE(fn) == SSA_NAME; k++) {
    gimple *d = SSA_NAME_DEF_STMT(fn);
    if (!d || !is_gimple_assign(d)) break;
    tree r = gimple_assign_rhs1(d);
    if (!s.empty()) s += " <- ";
    s += expr(r);
    if (gimple_assign_rhs_code(d) == POINTER_PLUS_EXPR) { s += " + " + expr(gimple_assign_rhs2(d)); }
    if (TREE_CODE(r) == SSA_NAME) fn = r;
    else if (TREE_CODE(r) == COMPONENT_REF || TREE_CODE(r) == MEM_REF) {
      tree b = r; while (TREE_CODE(b) == COMPONENT_REF) b = TREE_OPERAND(b, 0);
      if (TREE_CODE(b) == MEM_REF) { fn = TREE_OPERAND(b, 0); } else break;
    } else break;
  }
  for (auto &c : s) if (c == '\t' || c == '\n') c = ' ';
  return s;
}
namespace {
const pass_data pd = {GIMPLE_PASS, "callscan", OPTGROUP_NONE, TV_NONE, PROP_cfg, 0, 0, 0, 0};
struct callscan : gimple_opt_pass {
  callscan(gcc::context *c) : gimple_opt_pass(pd, c) {}
  unsigned int execute(function *f) override {
    basic_block bb;
    const char *caller = IDENTIFIER_POINTER(DECL_ASSEMBLER_NAME(f->decl));
    unsigned nth = 0;
    FOR_EACH_BB_FN(bb, f)
      for (gimple_stmt_iterator gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
        gimple *g = gsi_stmt(gsi);
        if (!is_gimple_call(g) || gimple_call_internal_p(g)) continue;
        tree fndecl = gimple_call_fndecl(g);
        if (fndecl && fndecl_built_in_p(fndecl)) continue;
        tree ft = gimple_call_fntype(g);
        expanded_location el = expand_location(gimple_location(g));
        std::string params;
        bool proto = ft && prototype_p(ft), var = ft && stdarg_p(ft);
        if (ft) for (tree a = TYPE_ARG_TYPES(ft); a && TREE_VALUE(a) != void_type_node; a = TREE_CHAIN(a))
          params += (params.empty() ? "" : ",") + tcls(TREE_VALUE(a));
        std::string dsig = "";  // a direct call: the callee declaration's own type (differs from fntype: a cast call)
        if (fndecl) {
          tree dt = TREE_TYPE(fndecl);
          dsig = tcls(TREE_TYPE(dt)) + (prototype_p(dt) ? "(" : "K(");
          bool first = true;
          for (tree a = TYPE_ARG_TYPES(dt); a && TREE_VALUE(a) != void_type_node; a = TREE_CHAIN(a)) { dsig += (first ? "" : ",") + tcls(TREE_VALUE(a)); first = false; }
          dsig += stdarg_p(dt) ? ",...)" : ")";
        }
        std::string args, argx;
        for (unsigned i = 0; i < gimple_call_num_args(g); i++) {
          tree a = gimple_call_arg(g, i);
          args += (i ? "," : "") + tcls(TREE_TYPE(a));
          // an int argument that is a reinterpretation of a float (VIEW_CONVERT / a load through an int lvalue of a float object)
          std::string x = "";
          if (TREE_CODE(a) == SSA_NAME && SSA_NAME_DEF_STMT(a) && is_gimple_assign(SSA_NAME_DEF_STMT(a))) {
            gimple *d = SSA_NAME_DEF_STMT(a);
            tree r = gimple_assign_rhs1(d);
            if (TREE_CODE(r) == VIEW_CONVERT_EXPR) x = "VC:" + tcls(TREE_TYPE(TREE_OPERAND(r, 0)));
          }
          argx += (i ? "," : "") + x;
        }
        tree lhs = gimple_call_lhs(g);
        unsigned id = 0;
        if (!fndecl) {
          char k[64]; snprintf(k, sizeof k, ":%u:%d:%d", nth++, el.line, el.column);
          id = fnv(k, fnv(caller, fnv(el.file ? el.file : "?", 2166136261u)));
          if (probe && gimple_vuse(g)) {
            if (!probe_decl) {
              tree t = build_function_type_list(void_type_node, unsigned_type_node, ptr_type_node, NULL_TREE);
              probe_decl = build_fn_decl("__icall_probe", t);
            }
            gcall *pc = gimple_build_call(probe_decl, 2, build_int_cst(unsigned_type_node, id), gimple_call_fn(g));
            gimple_set_location(pc, gimple_location(g));
            gimple_set_vuse(pc, gimple_vuse(g));
            tree vd = make_ssa_name(gimple_vop(f), pc);
            gimple_set_vdef(pc, vd);
            gimple_set_vuse(g, vd);
            gsi_insert_before(&gsi, pc, GSI_SAME_STMT);
          }
        }
        fprintf(out, "%08x\t%s:%d:%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", id, el.file ? el.file : "?", el.line, el.column, caller,
                fndecl ? IDENTIFIER_POINTER(DECL_ASSEMBLER_NAME(fndecl)) : "*",
                fndecl ? dsig.c_str() : where(gimple_call_fn(g)).c_str(),
                ft ? tcls(TREE_TYPE(ft)).c_str() : "?", proto ? "P" : "K", var ? "V" : "-", params.c_str(), args.c_str(), argx.c_str(),
                lhs ? tcls(TREE_TYPE(lhs)).c_str() : "-", ft ? tname(ft).c_str() : "?");
      }
    return 0;
  }
};
}
// type punning of a scalar / aggregate decl: overlapping memory accesses of different sizes (byte order)
struct acc { tree decl; long off, size; bool write; std::string ty; int line; };
static void collect(tree ref, bool write, int line, std::vector<acc> &v) {
  if (!ref || TREE_CODE(ref) == SSA_NAME || CONSTANT_CLASS_P(ref)) return;
  if (TREE_CODE(ref) == BIT_FIELD_REF && TREE_CODE(TREE_OPERAND(ref, 0)) == SSA_NAME && !write) {
    tree sn = TREE_OPERAND(ref, 0), var = SSA_NAME_VAR(sn);
    if (var && DECL_P(var) && tree_fits_uhwi_p(TREE_OPERAND(ref, 1)) && tree_fits_uhwi_p(TREE_OPERAND(ref, 2))) {
      tree t = TREE_TYPE(sn);
      v.push_back({var, 0, (long)tree_to_uhwi(TYPE_SIZE(t)), true, tcls(t), 0});
      v.push_back({var, (long)tree_to_uhwi(TREE_OPERAND(ref, 2)), (long)tree_to_uhwi(TREE_OPERAND(ref, 1)), false, tcls(TREE_TYPE(ref)), line});
    }
    return;
  }
  poly_int64 bs, bz, mx; bool rev;
  if ((TREE_CODE(ref) == MEM_REF || TREE_CODE(ref) == TARGET_MEM_REF) && TREE_CODE(TREE_OPERAND(ref, 0)) == SSA_NAME
      && TREE_CODE(ref) == MEM_REF && tree_fits_shwi_p(TREE_OPERAND(ref, 1))) {
    // *p with p = &decl (+ const), chased through the early SSA temporaries
    long off = tree_to_shwi(TREE_OPERAND(ref, 1)) * 8;
    tree p = TREE_OPERAND(ref, 0);
    for (int k = 0; k < 6 && p && TREE_CODE(p) == SSA_NAME; k++) {
      gimple *d = SSA_NAME_DEF_STMT(p);
      if (!d || !is_gimple_assign(d)) { p = 0; break; }
      enum tree_code c = gimple_assign_rhs_code(d);
      tree r1 = gimple_assign_rhs1(d);
      if (c == POINTER_PLUS_EXPR && tree_fits_shwi_p(gimple_assign_rhs2(d))) { if (off >= 0) off += tree_to_shwi(gimple_assign_rhs2(d)) * 8; p = r1; }
      else if (c == POINTER_PLUS_EXPR) { off = -1000000; p = r1; }
      else if (c == SSA_NAME || CONVERT_EXPR_CODE_P(c)) p = r1;
      else if (c == ADDR_EXPR) { p = r1; break; }
      else { p = 0; break; }
    }
    if (p && TREE_CODE(p) == ADDR_EXPR) {
      HOST_WIDE_INT o2, z2;
      tree b2 = get_ref_base_and_extent(TREE_OPERAND(p, 0), &bs, &bz, &mx, &rev);
      tree t = TREE_TYPE(ref);
      if (b2 && DECL_P(b2) && TREE_CODE(b2) != FUNCTION_DECL && bs.is_constant(&o2) && TYPE_SIZE(t) && tree_fits_uhwi_p(TYPE_SIZE(t))) {
        z2 = tree_to_uhwi(TYPE_SIZE(t));
        if (z2 > 0) v.push_back({b2, off < 0 ? -1 : (long)(o2 + off), (long)z2, write, tcls(t), line});
      }
      return;
    }
  }
  tree base = get_ref_base_and_extent(ref, &bs, &bz, &mx, &rev);
  if (!base || !DECL_P(base) || TREE_CODE(base) == FUNCTION_DECL) return;
  HOST_WIDE_INT o, z;
  if (!bs.is_constant(&o) || !bz.is_constant(&z) || z <= 0) return;
  tree t = TREE_TYPE(ref);
  v.push_back({base, (long)o, (long)z, write, tcls(t), line});
}
const pass_data pd2 = {GIMPLE_PASS, "punscan", OPTGROUP_NONE, TV_NONE, PROP_cfg, 0, 0, 0, 0};
struct punscan : gimple_opt_pass {
  punscan(gcc::context *c) : gimple_opt_pass(pd2, c) {}
  unsigned int execute(function *f) override {
    basic_block bb; std::vector<acc> v;
    const char *caller = IDENTIFIER_POINTER(DECL_ASSEMBLER_NAME(f->decl));
    FOR_EACH_BB_FN(bb, f)
      for (gimple_stmt_iterator gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
        gimple *g = gsi_stmt(gsi);
        int line = expand_location(gimple_location(g)).line;
        if (gimple_clobber_p(g)) continue;   // an end-of-scope clobber, not a write
        if (is_gimple_assign(g)) {
          collect(gimple_assign_lhs(g), true, line, v);
          for (unsigned i = 1; i < gimple_num_ops(g); i++) {
            tree r = gimple_op(g, i);
            if (r && TREE_CODE(r) == ADDR_EXPR) continue;
            collect(r, false, line, v);
          }
        } else if (is_gimple_call(g)) {
          collect(gimple_call_lhs(g), true, line, v);
          for (unsigned i = 0; i < gimple_call_num_args(g); i++) {
            tree r = gimple_call_arg(g, i);
            if (TREE_CODE(r) != ADDR_EXPR) collect(r, false, line, v);
          }
        }
      }
    // parameters: an implicit write of the declared type at entry
    for (tree p = DECL_ARGUMENTS(f->decl); p; p = DECL_CHAIN(p)) {
      tree t = TREE_TYPE(p);
      if (TYPE_SIZE(t) && tree_fits_uhwi_p(TYPE_SIZE(t)))
        v.push_back({p, 0, (long)tree_to_uhwi(TYPE_SIZE(t)), true, tcls(t), 0});
    }
    // per decl: a scalar read overlapping a scalar write of ANOTHER size (the bytes depend on the byte order)
    auto scalar = [](const std::string &c) { return !c.empty() && (c[0] == 'i' || c[0] == 'f' || c[0] == 'p' || c[0] == 't'); };
    for (size_t i = 0; i < v.size(); i++) {
      if (v[i].write || !scalar(v[i].ty)) continue;
      tree d = v[i].decl;
      for (size_t j = 0; j < v.size(); j++) {
        if (!v[j].write || v[j].decl != d || !scalar(v[j].ty)) continue;
        bool ov = v[i].off < 0 || v[j].off < 0 || (v[i].off < v[j].off + v[j].size && v[j].off < v[i].off + v[i].size);
        if (!ov || v[i].size == v[j].size) continue;
        fprintf(pout, "PUN\t%s:%d\t%s\t%s\t%s\t%s\tw%s@%ld/%ld:%d\tr%s@%ld/%ld:%d\n", main_input_filename, v[i].line, caller,
                DECL_NAME(d) ? IDENTIFIER_POINTER(DECL_NAME(d)) : "?", TREE_CODE(d) == PARM_DECL ? "parm" : (TREE_STATIC(d) || DECL_EXTERNAL(d) ? "global" : "local"),
                tcls(TREE_TYPE(d)).c_str(), v[j].ty.c_str(), v[j].off / 8, v[j].size / 8, v[j].line, v[i].ty.c_str(), v[i].off / 8, v[i].size / 8, v[i].line);
        break;
      }
    }
    // an access through &param beyond the parameter (the O32 home-slot idiom `*(Pair2 *)&a1` reading a1 and a2):
    // on MIPS and i386 the next argument's home follows; on x86-64 the register arguments are spilled anywhere
    for (auto &a : v) {
      if (TREE_CODE(a.decl) != PARM_DECL || !a.line) continue;
      tree t = TREE_TYPE(a.decl);
      if (!TYPE_SIZE(t) || !tree_fits_uhwi_p(TYPE_SIZE(t))) continue;
      long psz = tree_to_uhwi(TYPE_SIZE(t));
      if (a.off >= 0 && a.off + a.size <= psz) continue;
      fprintf(pout, "HOME\t%s:%d\t%s\t%s\tparm\t%s\t%s\t%s@%ld/%ld\n", main_input_filename, a.line, caller,
              DECL_NAME(a.decl) ? IDENTIFIER_POINTER(DECL_NAME(a.decl)) : "?", tcls(t).c_str(), a.write ? "write" : "read",
              a.ty.c_str(), a.off / 8, a.size / 8);
    }
    // a by-value aggregate parameter: list it with its field accesses
    for (tree p = DECL_ARGUMENTS(f->decl); p; p = DECL_CHAIN(p)) {
      tree t = TREE_TYPE(p);
      if (TREE_CODE(t) == RECORD_TYPE || TREE_CODE(t) == UNION_TYPE) {
        std::string reads;
        for (auto &a : v) if (a.decl == p && !a.write) { char b[64]; snprintf(b, sizeof b, " %s@%ld", a.ty.c_str(), a.off / 8); if (reads.find(b) == std::string::npos) reads += b; }
        fprintf(pout, "AGGPARM\t%s:%d\t%s\t%s\t%s\t%s\n", main_input_filename, expand_location(DECL_SOURCE_LOCATION(p)).line, caller,
                DECL_NAME(p) ? IDENTIFIER_POINTER(DECL_NAME(p)) : "?", tcls(t).c_str(), reads.c_str());
      }
    }
    tree rt = TREE_TYPE(TREE_TYPE(f->decl));
    if (TREE_CODE(rt) == RECORD_TYPE || TREE_CODE(rt) == UNION_TYPE)
      fprintf(pout, "AGGRET\t%s\t%s\t%s\n", main_input_filename, caller, tcls(rt).c_str());
    return 0;
  }
};
int plugin_init(struct plugin_name_args *pi, struct plugin_gcc_version *ver) {
  const char *o = 0;
  for (int i = 0; i < pi->argc; i++) { if (!strcmp(pi->argv[i].key, "out")) o = pi->argv[i].value; if (!strcmp(pi->argv[i].key, "probe")) probe = 1; }
  if (!o) return 1;
  char path[4096]; std::string base = main_input_filename ? main_input_filename : "x";
  for (auto &c : base) if (c == '/') c = '_';
  snprintf(path, sizeof path, "%s/%s.calls", o, base.c_str());
  out = fopen(path, "w"); if (!out) return 1;
  snprintf(path, sizeof path, "%s/%s.pun", o, base.c_str());
  pout = fopen(path, "w"); if (!pout) return 1;
  struct register_pass_info rp2;
  rp2.pass = new punscan(g);
  rp2.reference_pass_name = "cfg"; rp2.ref_pass_instance_number = 1; rp2.pos_op = PASS_POS_INSERT_AFTER;
  register_callback(pi->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &rp2);
  struct register_pass_info rp;
  rp.pass = new callscan(g);
  rp.reference_pass_name = "optimized"; rp.ref_pass_instance_number = 1; rp.pos_op = PASS_POS_INSERT_AFTER;
  register_callback(pi->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &rp);
  return 0;
}
