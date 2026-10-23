/* Native original-compiler source port from src/game_libs/game_libs_post.c. Target bytes unchanged. */
#include "context_game_libs_post.h"
void gl_func_0002D910(int a0, int a1, int a2, int a3)
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
  if (D_0001C1BC.field_1C1BC != (*primary_input_home))
  {
    if ((D_0001C1CC.field_1C1CC == 0) && (a0 == 0))
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
    if (D_0001C1C0.field_1C1C0 != (*secondary_input_home))
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
  v0 = D_0001C1C4.field_1C1C4;
  D_0001C1BC_b.field_1C1BC = a0;
  D_0001C1C0_b.field_1C1C0 = a1;
  v0 = (v0 + a2) / 2;
  D_0001C1C8_b.field_1C1C8 = (D_0001C1C8.field_1C1C8 + a3) / 2;
  if (D_0001C1D4.field_1C1D4 == 0)
  {
    D_0001C1CC_g.field_1C1CC = -1;
    if (v0 < (-8))
    {
      D_0001C1D4_b.field_1C1D4 = -1;
      D_0001C1D0.field_1C1D0 = v0;
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
