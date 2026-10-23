/* Native original-compiler source port from src/game_libs/game_libs_post0b.c. Target bytes unchanged. */
#include "context_game_libs_post0b.h"
void gl_func_0004C928(struct GLRender4C *arg0, struct Arg4C928 *arg1)
{
  unsigned int loop_ready;
  unsigned int ready;
  struct StackB b;
  struct Obj4C928 *receiver0;
  union { struct St4C928 *state; struct GlSmallState22C *small; struct GLMethods4C *methods; s32 saved_d0; void *queue; } work;
  struct GLMethods4C *table0;
  struct Obj4C928 *receiver1;
  struct GLMethods4C *table1;
  struct GLRequest4C a0blk;
  struct Obj4C928 *receiver2;
  struct GLRequest4C a1blk;
  struct GLRequest4C a2blk;
  struct GLMethods4C *table2;
  work.state = *((struct St4C928 **) (((char *) (&gl_4C928_state_base)) + 0x218));
  do
  {
    do
    {
      ready = work.state->p1C != 0;
      if (ready == 0)
      {
        ready = work.state->p04 != 0;
        if (ready == 0)
        {
          ready = work.state->p0C != 0;
        }
      }
    }
    while (0);
    if (ready != 0)
    {
      do
      {
        gl_4C928_poll();
        do
        {
          ready = work.state->p1C != 0;
          if (ready == 0)
          {
            ready = work.state->p04 != 0;
            if (ready == 0)
            {
              ready = (*work.state).p0C != 0;
            }
          }
        }
        while (0);
      }
      while (ready != 0);
    }
  }
  while (0);
  work.small = *((struct GlSmallState22C **) (((char *) (&gl_4C928_state_base)) + 0x22C));
  work.small->p0C = 0;
  work.methods = *((struct GLMethods4C **) (((char *) arg0) + 0x28));
  gl_4C928_result_base = work.methods->m64(work.methods->h60 + (char *)arg0);
  ready = arg0->dD4;
  work.saved_d0 = arg0->dD0;
  arg0->dD0 = (s32) (4.0f - arg1->fBC);
  arg0->dD4 = (s32) (4.0f - arg1->fC0);
  *((s32 *) (((char *) (&gl_4C928_state_base)) + 0x1C4)) = (*((s32 *) (((char *) (&gl_4C928_state_base)) + 0x1C4))) | 2;
  gl_4C928_process(arg0);
  *(s32 *)((char *)&gl_4C928_state_base + 0x1C4) &= ~2;
  arg0->dD0 = work.saved_d0;
  arg0->dD4 = ready;
  gl_4C928_mode(arg0, 1);
  work.state = *((struct St4C928 **) (((char *) (&gl_4C928_state_base)) + 0x218));
  do
  {
    do
    {
      ready = work.state->p1C != 0;
      if (ready == 0)
      {
        ready = work.state->p04 != 0;
        if (ready == 0)
        {
          ready = work.state->p0C != 0;
        }
      }
    }
    while (0);
    if (ready != 0)
    {
      do
      {
        gl_4C928_poll();
        do
        {
          loop_ready = work.state->p1C != 0;
          if (loop_ready == 0)
          {
            loop_ready = work.state->p04 != 0;
            if (loop_ready == 0)
            {
              loop_ready = work.state->p0C != 0;
            }
          }
        }
        while (0);
      }
      while (loop_ready != 0);
    }
  }
  while (0);
  work.queue = *(void **)((char *)&gl_4C928_queue_base + 0x1C);
  gl_4C928_send(work.queue, 0x68, 1);
  work.state = *((struct St4C928 **) (((char *) (&gl_4C928_state_base)) + 0x218));
  do
  {
    do
    {
      ready = work.state->p1C != 0;
      if (ready == 0)
      {
        ready = work.state->p04 != 0;
        if (ready == 0)
        {
          ready = work.state->p0C != 0;
        }
      }
    }
    while (0);
    if (ready != 0)
    {
      do
      {
        gl_4C928_poll();
        do
        {
          loop_ready = work.state->p1C != 0;
          if (loop_ready == 0)
          {
            loop_ready = work.state->p04 != 0;
            if (loop_ready == 0)
            {
              loop_ready = work.state->p0C != 0;
            }
          }
        }
        while (0);
      }
      while (loop_ready != 0);
    }
  }
  while (0);
  if (arg1->p10C != 0)
  {
    ready = arg0->d1A8;
    do { loop_ready = arg0->h1AC; } while (0);
    b.b = 0;
    a0blk.n = 0xC; a0blk.p = &b;
    b.c = ready;
    b.a = (s16)loop_ready;
    do {
      receiver0 = arg1->p10C;
      table0 = receiver0->methods;
      table0->m2C(table0->h28 + (char *)receiver0, &a0blk);
    } while (0);
  }
  arg0->d1A8 = gl_4C928_result_base->h48;
  do { arg0->d1A8 = arg0->d1A8 >> 1; } while (0);
  arg1->p10C = (struct Obj4C928 *) gl_4C928_find(arg0, arg0->d1A8);
  gl_4C928_log((char *)&gl_4C928_strings_base + 0x2017C, arg0->d1A8);
  *((s32 *) (((char *) (&gl_4C928_state_base)) + 0x1C8)) = (s32) arg1->p10C;
  if (arg1->p10C != 0)
  {
    gl_4C928_log((char *)&gl_4C928_strings_base + 0x20188, arg1->p10C->id);
    a1blk.n = 4;
    a1blk.p = arg1;
    do {
      receiver1 = arg1->p10C;
      table1 = receiver1->methods;
      table1->m2C(table1->h28 + (char *)receiver1, &a1blk);
    } while (0);
    {
      ready = arg0->d1A8;
      loop_ready = arg0->h1AC;
      b.b = 1;
      a2blk.n = 0xC; a2blk.p = &b;
      b.c = ready;
      b.a = (s16) loop_ready;
    }
    do {
      receiver2 = arg1->p10C;
      table2 = receiver2->methods;
      table2->m2C(table2->h28 + (char *)receiver2, &a2blk);
    } while (0);
  }
  work.small = *((struct GlSmallState22C **) (((char *) (&gl_4C928_state_base)) + 0x22C));
  work.small->p0C = 0;
}
