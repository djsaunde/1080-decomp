#ifndef GDL_HUD_H
#define GDL_HUD_H

/* The race HUD object that owns the "Gadget" widgets (game_libs 0xE2A4-0xEAAC,
 * src/game_libs/game_libs_tail.c). Each hud_add<X>Gadget lazily builds the
 * sprite sets it needs (hud_initSprites_*), constructs the gadget with
 * <X>Gadget_ctor(0, ...), stores it in its slot, then links it as a child
 * through the node child list at +0x10 (list_append) and sets the
 * gadget's parent back-link (gadget +0x14). hud_releaseGadgets calls each
 * slot's <X>Gadget_release.
 *
 * Only the fields the hud_* functions touch are named. Slots and value
 * pointers are kept as int where the matched bodies use int arithmetic;
 * a field's width and signedness must not change (READABILITY_PLAN 9.1). */
#include "gdl/race.h"

/* The HUD object is the RaceTracker (include/gdl/race.h; RaceTracker_ctor allocs
 * 0xB8 and its update dispatches the speed / time / points / cool gadgets at
 * 0x70..0x9C). Hud is kept as the name the hud_* functions use. */
typedef RaceTracker Hud;

#endif /* GDL_HUD_H */
