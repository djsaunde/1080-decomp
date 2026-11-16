#ifndef GDL_PARM_H
#define GDL_PARM_H

/* s32 from common.h (or a match_imports context header): not included here */

/* libgdl HostIO parms: the base every parm type is built on (agent-b run 566).
 *
 * Parms_addParm(parm, parms, tag, 1) (game_libs 0x35B1C) is the parm base ctor: it allocates
 * the 0x10-byte base when parm is NULL, stores the base type vtable at +0xC, clears the tag
 * member, stores the 4th argument at +0x8, fails with "Dup parm" (sFmt_Dup_parm_1E6B0) when a
 * parm with the same tag is already on the owner's list, and appends the parm at the list tail.
 * The caller then stores its own type vtable at +0xC and the value from +0x10 on (gdl/gadget.h
 * GdlParm / GdlParmF / GdlParmRGBA, gdl/dnode.h GdlParmV3, gdl/light.h BaseLightParms). */

/* the 4-char tag ("xps", "col", ...): passed and copied by value as a 4-byte struct */
typedef struct GdlParmTag {
    s32 v;
} GdlParmTag;

typedef struct GdlParmBase {
    /* 0x00 */ struct GdlParmBase *next; /* owner list link (0 = last) */
    /* 0x04 */ GdlParmTag tag;
    /* 0x08 */ s32 unk08;                /* Parms_addParm's 4th argument (1 at every call seen) */
    /* 0x0C */ void *type;               /* parm type vtable */
} GdlParmBase;

/* the 8-byte head of a "<Class>::Parms" object (gdl/gadget.h GdlParms) seen as the parm list */
typedef struct GdlParmList {
    /* 0x00 */ const char *name;         /* "<Class>::Parms" */
    /* 0x04 */ GdlParmBase *first;       /* 0 until the first Parms_addParm */
} GdlParmList;

#endif
