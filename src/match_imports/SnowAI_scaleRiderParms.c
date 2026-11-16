/* Native original-compiler source port from src/game_uso/game_uso.c. Target bytes unchanged. */
#include "context_game_uso.h"
#include "game/snowai.h"

#define AI(p) ((SnowAI *)(p))
#define PARM(p) ((SnowBParm *)(p))

/* SnowAI_scaleRiderParms(ai): save the rider's max / nts / dts / child nbf parm values in
 * ai->savedMax..savedNbf, scale max by dd2 and nts / dts by dd4, and switch nbf to dbf.
 * The `slot = base; if (1) { slot += K; }` steps are load-bearing (game_uso.c's note). */
void SnowAI_scaleRiderParms(char *a0) {
    char *snapshot_slot;
    char *linked;
    char *replacement_slot;
    char *factor_slot;
    char *write_slot;
    snapshot_slot = a0;
    while (0) { snapshot_slot += 1; }
    linked = (char *)AI(a0)->rider;
    write_slot = a0;
    while (0) { write_slot += 1; }
    snapshot_slot = linked;
    if (1) { snapshot_slot += 0x758; } /* &rider->parms.max */
    AI(a0)->savedMax = PARM(snapshot_slot)->value;
    snapshot_slot = linked;
    if (1) { snapshot_slot += 0x6F8; } /* &rider->parms.nts */
    AI(a0)->savedNts = PARM(snapshot_slot)->value;
    snapshot_slot = linked;
    if (1) { snapshot_slot += 0x6E0; } /* &rider->parms.dts */
    AI(a0)->savedDts = PARM(snapshot_slot)->value;
    snapshot_slot = linked;
    if (1) { snapshot_slot += 0x498; } /* &rider->child.parms.nbf */
    replacement_slot = write_slot;
    factor_slot = a0;
    if (1) { factor_slot += 0x35C; } /* &ai->parms.parm[SNOWAI_PARM_DD2 + 2] (dd4) */
    AI(a0)->savedNbf = PARM(snapshot_slot)->value;
    ((SnowBoarder *)linked)->parms.max.value = (*AI(a0)).parms.parm[SNOWAI_PARM_DD2].value * (*AI(a0)).savedMax;
    write_slot = (char *)&AI(a0)->rider->parms.nts;
    PARM(write_slot)->value = (*AI(a0)).parms.parm[SNOWAI_PARM_DD2 + 2].value * (*AI(a0)).savedNts;
    if (replacement_slot) {}
    write_slot = (char *)&AI(a0)->rider->parms.dts;
    PARM(write_slot)->value = (*(SnowAIParm *)factor_slot).value * (*AI(a0)).savedDts;
    linked = (char *)AI(a0)->rider;
    replacement_slot = linked;
    if (1) { replacement_slot += 0x4C8; } /* &rider->child.parms.dbf */
    write_slot = linked;
    if (1) { write_slot += 0x498; } /* &rider->child.parms.nbf */
    PARM(write_slot)->value = PARM(replacement_slot)->value;
}
