/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
#ifdef PORT /* one object for the bytes these tokens share, so gcc keeps the order (scripts/port-alias-audit.py, plan 18.7) */
extern char port_bootup_data[];
#define D_0001C1BC (*(__typeof__(D_0001C1BC) *)(port_bootup_data + 0x0))
#define D_0001C1BC_b (*(__typeof__(D_0001C1BC_b) *)(port_bootup_data + 0x0))
#define D_0001C1C0 (*(__typeof__(D_0001C1C0) *)(port_bootup_data + 0x0))
#define D_0001C1C0_b (*(__typeof__(D_0001C1C0_b) *)(port_bootup_data + 0x0))
#define D_0001C1C4 (*(__typeof__(D_0001C1C4) *)(port_bootup_data + 0x0))
#define D_0001C1C4_b (*(__typeof__(D_0001C1C4_b) *)(port_bootup_data + 0x0))
#define D_0001C1C4_c (*(__typeof__(D_0001C1C4_c) *)(port_bootup_data + 0x0))
#define D_0001C1C4_d (*(__typeof__(D_0001C1C4_d) *)(port_bootup_data + 0x0))
#define D_0001C1C4_e (*(__typeof__(D_0001C1C4_e) *)(port_bootup_data + 0x0))
#define D_0001C1C4_f (*(__typeof__(D_0001C1C4_f) *)(port_bootup_data + 0x0))
#define D_0001C1C4_g (*(__typeof__(D_0001C1C4_g) *)(port_bootup_data + 0x0))
#define D_0001C1C4_h (*(__typeof__(D_0001C1C4_h) *)(port_bootup_data + 0x0))
#define D_0001C1C4_i (*(__typeof__(D_0001C1C4_i) *)(port_bootup_data + 0x0))
#define D_0001C1C4_j (*(__typeof__(D_0001C1C4_j) *)(port_bootup_data + 0x0))
#define D_0001C1C8 (*(__typeof__(D_0001C1C8) *)(port_bootup_data + 0x0))
#define D_0001C1C8_b (*(__typeof__(D_0001C1C8_b) *)(port_bootup_data + 0x0))
#define D_0001C1C8_c (*(__typeof__(D_0001C1C8_c) *)(port_bootup_data + 0x0))
#define D_0001C1CC (*(__typeof__(D_0001C1CC) *)(port_bootup_data + 0x0))
#define D_0001C1CC_b (*(__typeof__(D_0001C1CC_b) *)(port_bootup_data + 0x0))
#define D_0001C1CC_c (*(__typeof__(D_0001C1CC_c) *)(port_bootup_data + 0x0))
#define D_0001C1CC_d (*(__typeof__(D_0001C1CC_d) *)(port_bootup_data + 0x0))
#define D_0001C1CC_e (*(__typeof__(D_0001C1CC_e) *)(port_bootup_data + 0x0))
#define D_0001C1CC_f (*(__typeof__(D_0001C1CC_f) *)(port_bootup_data + 0x0))
#define D_0001C1CC_g (*(__typeof__(D_0001C1CC_g) *)(port_bootup_data + 0x0))
#define D_0001C1CC_h (*(__typeof__(D_0001C1CC_h) *)(port_bootup_data + 0x0))
#define D_0001C1CC_i (*(__typeof__(D_0001C1CC_i) *)(port_bootup_data + 0x0))
#define D_0001C1CC_j (*(__typeof__(D_0001C1CC_j) *)(port_bootup_data + 0x0))
#define D_0001C1CC_k (*(__typeof__(D_0001C1CC_k) *)(port_bootup_data + 0x0))
#define D_0001C1D0 (*(__typeof__(D_0001C1D0) *)(port_bootup_data + 0x0))
#define D_0001C1D0_b (*(__typeof__(D_0001C1D0_b) *)(port_bootup_data + 0x0))
#define D_0001C1D0_c (*(__typeof__(D_0001C1D0_c) *)(port_bootup_data + 0x0))
#define D_0001C1D0_d (*(__typeof__(D_0001C1D0_d) *)(port_bootup_data + 0x0))
#define D_0001C1D0_e (*(__typeof__(D_0001C1D0_e) *)(port_bootup_data + 0x0))
#define D_0001C1D0_f (*(__typeof__(D_0001C1D0_f) *)(port_bootup_data + 0x0))
#define D_0001C1D0_g (*(__typeof__(D_0001C1D0_g) *)(port_bootup_data + 0x0))
#define D_0001C1D0_h (*(__typeof__(D_0001C1D0_h) *)(port_bootup_data + 0x0))
#define D_0001C1D4 (*(__typeof__(D_0001C1D4) *)(port_bootup_data + 0x0))
#define D_0001C1D4_b (*(__typeof__(D_0001C1D4_b) *)(port_bootup_data + 0x0))
#define D_0001C1D4_c (*(__typeof__(D_0001C1D4_c) *)(port_bootup_data + 0x0))
#define D_0001C1D4_d (*(__typeof__(D_0001C1D4_d) *)(port_bootup_data + 0x0))
#define D_0001C1D4_e (*(__typeof__(D_0001C1D4_e) *)(port_bootup_data + 0x0))
#define D_0001C1D4_f (*(__typeof__(D_0001C1D4_f) *)(port_bootup_data + 0x0))
#define D_0001C1D4_g (*(__typeof__(D_0001C1D4_g) *)(port_bootup_data + 0x0))
#endif
void snd_SetPlayer1Channel15IO_2D910(int a0, int a1, int a2, int a3)
{
  int v0;
  int *primary_input_home;
  int *secondary_input_home;
  if (gl_2D910_mode != 10)
  {
    return;
  }
  secondary_input_home = &a1;
  primary_input_home = &a0;
  if (gl_2D910_ready != 1)
  {
    return;
  }
  if (D_0001C1BC.field_1C1BC != (*primary_input_home)) /* UNNAMED: data-base D_0001C1BC is the game_libs Data base placeholder (link value 0, field_1C1BC = Data+0x1C1BC; ROM HI/LO Data(base)) */
  {
    if ((D_0001C1CC.field_1C1CC == 0) && (a0 == 0)) /* UNNAMED: data-base D_0001C1CC is the game_libs Data base placeholder (link value 0, field_1C1CC = Data+0x1C1CC; ROM HI/LO Data(base)) */
    {
      gl_2D910_set_control(0x06010F00, 2);
      D_0001C1CC_b.field_1C1CC = -1;
    }
    else
      if (a0 == 1)
    {
      gl_2D910_set_control(0x06010F00, 0);
      D_0001C1CC_c.field_1C1CC = 0;
    }
  }
  else
    if (D_0001C1C0.field_1C1C0 != (*secondary_input_home)) /* UNNAMED: data-base D_0001C1C0 is the game_libs Data base placeholder (link value 0, field_1C1C0 = Data+0x1C1C0; ROM HI/LO Data(base)) */
  {
    if ((1 == D_0001C1CC_d.field_1C1CC) && (a1 == 0))
    {
      gl_2D910_set_control(0x06010F00, 2);
      D_0001C1CC_e.field_1C1CC = -1;
    }
    else
      if (a1 == 1)
    {
      gl_2D910_set_control_byte(0x06010F00, 1);
      D_0001C1CC_f.field_1C1CC = 1;
    }
  }
  v0 = D_0001C1C4.field_1C1C4; /* UNNAMED: data-base D_0001C1C4 is the game_libs Data base placeholder (link value 0, field_1C1C4 = Data+0x1C1C4; ROM HI/LO Data(base)) */
  D_0001C1BC_b.field_1C1BC = a0;
  D_0001C1C0_b.field_1C1C0 = a1;
  v0 = (v0 + a2) / 2;
  D_0001C1C8_b.field_1C1C8 = (D_0001C1C8.field_1C1C8 + a3) / 2; /* UNNAMED: data-base D_0001C1C8 is the game_libs Data base placeholder (link value 0, field_1C1C8 = Data+0x1C1C8; ROM HI/LO Data(base)) */
  if (D_0001C1D4.field_1C1D4 == 0) /* UNNAMED: data-base D_0001C1D4 is the game_libs Data base placeholder (link value 0, field_1C1D4 = Data+0x1C1D4; ROM HI/LO Data(base)) */
  {
    D_0001C1CC_g.field_1C1CC = -1;
    if (v0 < (-8))
    {
      D_0001C1D4_b.field_1C1D4 = -1;
      D_0001C1D0.field_1C1D0 = v0; /* UNNAMED: data-base D_0001C1D0 is the game_libs Data base placeholder (link value 0, field_1C1D0 = Data+0x1C1D0; ROM HI/LO Data(base)) */
    }
    D_0001C1C4_b.field_1C1C4 = v0;
    if (v0 >= 9)
    {
      D_0001C1D4_c.field_1C1D4 = 1;
      D_0001C1D0_b.field_1C1D0 = v0;
      D_0001C1C4_c.field_1C1C4 = v0;
    }
    {
      int c8;
      c8 = D_0001C1C8_c.field_1C1C8;
      c8 = (signed char) c8;
      gl_2D910_set_control(0x06010F02, c8);
    }
    v0 = D_0001C1C4_d.field_1C1C4;
  }
  if (1 == D_0001C1D4_d.field_1C1D4)
  {
    if (v0 < D_0001C1D0_c.field_1C1D0)
    {
      if (D_0001C1CC_h.field_1C1CC == 0xFFFFFFFFU)
      {
        D_0001C1C4_e.field_1C1C4 = v0;
        gl_2D910_set_control(0x06010F00, 1);
        gl_2D910_set_control(0x06010F02, (signed char) ((D_0001C1D0_d.field_1C1D0 * 4) - 0x40));
        D_0001C1CC_i.field_1C1CC = 1;
        v0 = D_0001C1C4_f.field_1C1C4;
      }
      if (v0 < 0x10)
      {
        D_0001C1D4_e.field_1C1D4 = 0;
      }
    }
    else
    {
      D_0001C1D0_e.field_1C1D0 = v0;
    }
  }
  D_0001C1C4_g.field_1C1C4 = v0;
  if (D_0001C1D4_f.field_1C1D4 == 0xFFFFFFFFU)
  {
    if (D_0001C1D0_f.field_1C1D0 < v0)
    {
      D_0001C1C4_h.field_1C1C4 = v0;
      if (D_0001C1CC_j.field_1C1CC == 0xFFFFFFFFU)
      {
        gl_2D910_set_control(0x06010F00, 0);
        gl_2D910_set_control(0x06010F02, (signed char) (((2 * (-D_0001C1D0_g.field_1C1D0)) * 2) - 0x40));
        D_0001C1CC_k.field_1C1CC = 0;
      }
      v0 = D_0001C1C4_i.field_1C1C4;
      if (v0 >= (-0xF))
      {
        D_0001C1D4_g.field_1C1D4 = 0;
      }
    }
    else
    {
      D_0001C1D0_h.field_1C1D0 = v0;
      D_0001C1C4_j.field_1C1C4 = v0;
    }
  }
}
#ifdef PORT
#undef D_0001C1BC
#undef D_0001C1BC_b
#undef D_0001C1C0
#undef D_0001C1C0_b
#undef D_0001C1C4
#undef D_0001C1C4_b
#undef D_0001C1C4_c
#undef D_0001C1C4_d
#undef D_0001C1C4_e
#undef D_0001C1C4_f
#undef D_0001C1C4_g
#undef D_0001C1C4_h
#undef D_0001C1C4_i
#undef D_0001C1C4_j
#undef D_0001C1C8
#undef D_0001C1C8_b
#undef D_0001C1C8_c
#undef D_0001C1CC
#undef D_0001C1CC_b
#undef D_0001C1CC_c
#undef D_0001C1CC_d
#undef D_0001C1CC_e
#undef D_0001C1CC_f
#undef D_0001C1CC_g
#undef D_0001C1CC_h
#undef D_0001C1CC_i
#undef D_0001C1CC_j
#undef D_0001C1CC_k
#undef D_0001C1D0
#undef D_0001C1D0_b
#undef D_0001C1D0_c
#undef D_0001C1D0_d
#undef D_0001C1D0_e
#undef D_0001C1D0_f
#undef D_0001C1D0_g
#undef D_0001C1D0_h
#undef D_0001C1D4
#undef D_0001C1D4_b
#undef D_0001C1D4_c
#undef D_0001C1D4_d
#undef D_0001C1D4_e
#undef D_0001C1D4_f
#undef D_0001C1D4_g
#endif
