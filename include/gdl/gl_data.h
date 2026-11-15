#ifndef GDL_GL_DATA_H
#define GDL_GL_DATA_H

/* game_libs (bootup.uso) data accesses: the `&D_00000000 + K` sites.
 *
 * The decomp links each USO at VRAM 0 and the ROM holds the PRE-relocation
 * instruction words, so every data reference is spelled off the zero
 * placeholder D_00000000. What K means depends on which bootup.uso Sym entry
 * the TextReloc at that instruction names (READABILITY_PLAN section 9,
 * "USO section-base accesses"):
 *
 *   Sym tag byte 0x?4 = a SECTION BASE (0x54 Text, 0x64 Data, 0x74 RoData,
 *     0x84 Bss; value 0): K is the offset inside that section. Two sites with
 *     the same K but different sections are different objects.
 *   Sym tag byte 0x?2 = a named OBJECT (0x62 Data, 0x72 RoData, 0x82 Bss;
 *     value = its section offset): the instruction immediates are 0, so K is
 *     only the field offset inside the object, and the object's identity is
 *     in the reloc table, not in the code.
 *
 * So no single struct can be overlaid at D_00000000. Instead, each object
 * gets a NAMED ACCESSOR MACRO whose expansion is exactly the expression the
 * matched body already used. The preprocessed source is then unchanged up to
 * redundant parentheses, which is what makes the conversion byte-safe; the
 * name records the reloc evidence (section, offset, Sym index).
 *
 * D_00000000 is declared per TU (int / char / void *); the macros only take
 * its address, so they work with any of those declarations. */

/* a site relocated against the Data section base Sym (tag 0x640000, sym 2) */
#define GL_DATA(off) ((char *)&D_00000000 + (off))
/* ... against the RoData / Bss section base Syms (tag 0x740000 sym 1, 0x840000 sym 4) */
#define GL_RODATA(off) ((char *)&D_00000000 + (off))
#define GL_BSS(off) ((char *)&D_00000000 + (off))
/* a section-base site whose K starts a string: the literal is documentation only (the
 * argument is dropped by the preprocessor, so the expansion is GL_DATA(off) exactly).
 * `uso-data-sites.py check` compares every literal with the ROM bytes at that offset, and
 * `spell-base` writes these (agent-a run 498). Parm tags are 3-char strings read as a word:
 * `*(s32 *)GL_DATA_STR(0x8748, "ied")`. */
#define GL_DATA_STR(off, text) GL_DATA(off)
#define GL_RODATA_STR(off, text) GL_RODATA(off)

/* ---- named bootup.uso objects (agent-a run 479; READABILITY_PLAN 9.1) ----
 *
 * OBJECT ALIASES: the name stands for the placeholder SYMBOL itself, so every
 * spelling the matched bodies use stays token-identical after preprocessing:
 *   &gHostio                  == &D_00000000
 *   (char *)&gGfxLib + 0xA0   == (char *)&D_00000000 + 0xA0
 *   INPUT_MGR(gInputMgr).pads == INPUT_MGR(D_00000000).pads   (game/input.h)
 * Only sites spelled with plain D_00000000 can use them. Per-site zero aliases
 * (gSysLib_415A4_s1, gSysLib_3c740, gGfxLib_4c5e4_flag, ...) exist to defeat IDO's
 * address CSE and must keep their own symbol; they are listed against their
 * object in READABILITY_PLAN 9.1, not replaced.
 *
 * Every comment names the bootup.uso Sym the TextRelocs point at.
 * `scripts/uso-data-sites.py check` verifies that each function using one of
 * these names really has a site relocated against that Sym; run it after a
 * conversion (with a built tree), then the readability gate.
 *
 * Sizes are inferred from the next named object in the section. */
#define gAudioContext D_00000000 /* Bss+0x345C0 Sym 1255, 0x5C30 bytes: the audio driver context (OoT gAudioContext; 133 AudioHeap/AudioLoad/AudioSeq functions) */
#define gGfxLib       D_00000000 /* Bss+0x3C168 Sym 154, 0x5D8 bytes: the GfxLib global (docs/1080_GRAPHICS.md section 2: +0xA0 modelview, +0x1C4 flags, +0x254 current ScreenWindow) */
#define gSysLib       D_00000000 /* Bss+0x3C740 Sym 73, 0x28 bytes: the system-services object passed as `this` to heap_tempAlloc*, the fs_gdlRead* parm readers, pad_testButton, sys_timeInFrames (169 functions); App_main queries its memory range first. Role inferred from callers */
#define gHostio       D_00000000 /* Bss+0x3C768 Sym 338, 8 bytes: the HostIO connection, first argument of every hostio_beginGroup/add/endGroup call in the 47 <Class>_hostioParms functions */
#define gWindowDefaults D_00000000 /* Bss+0x3EC20 Sym 324, 0x40 bytes: the values Window_setDefaults copies into every new Window (+0x8 width, +0xC height, +0x10 / +0x14 origin, +0x18 near, +0x1C far, +0x28 flags, +0x2C projType, +0x30 clearColour; window.h) */
#define gInputMgr     D_00000000 /* Bss+0xF8 Sym 272, 0x194 bytes: the InputMgr (include/game/input.h; 79 functions) */
#define gAudioHeap    D_00000000 /* Bss+0x4B0 Sym 1283, 0x34100 bytes: the audio heap arena (OoT gAudioHeap): AudioLoad_Init zeroes it, AudioHeap_InitMainPools carves initPool / miscPool from it (agent-a run 498) */
#define gAudioHeapInitSizes D_00000000 /* Data+0x1CDD0 Sym 1284, 16 bytes: OoT gAudioHeapInitSizes {+4 heapSize, +8 initPoolSize, +0xC permanentPoolSize}; AudioLoad_Init / AudioHeap_InitMainPools / AudioHeap_Init (agent-a run 498) */



/* ---- game_libs libultra globals (agent-a run 482) ----
 * game_libs statically links its own libultra (gl_ prefix, READABILITY_PLAN
 * section 4); these are that copy's globals, identified by which libultra
 * functions touch them and by the libultra 2.0I declaration order
 * (thread.c: __osRunQueue, __osActiveQueue, __osRunningThread are adjacent
 * here too). Most sites are per-site zero aliases, which keep their own
 * symbol and take `<name>_<suffix>` names. */
#define gl___osRunQueue        D_00000000 /* Data+0x2D698 Sym 2533: __osRunQueue (__osDispatchThread, __osExceptionPreamble, __osLeoResume) */
#define gl___osActiveQueue     D_00000000 /* Data+0x2D69C Sym 1557: __osActiveQueue (osCreateThread, osDestroyThread, dbg_printThreads) */
#define gl___osRunningThread   D_00000000 /* Data+0x2D6A0 Sym 2512: __osRunningThread (__osDispatchThread, __osExceptionPreamble, osDestroyThread) */
#define gl___osContinitialized D_00000000 /* Data+0x2D6C0 Sym 2561: __osContinitialized (osContInit: `if (it) return; it = 1;`) */
#define gl_osClockRate         D_00000000 /* Data+0x2D6E0 Sym 2579: osClockRate (u64, osInitialize) */
#define gl_osViClock           D_00000000 /* Data+0x2D6E8 Sym 2580: osViClock (osInitialize, osAiSetFrequency) */
#define gl___OSGlobalIntMask   D_00000000 /* Data+0x2D6F0 Sym 2517: __OSGlobalIntMask (__osDispatchThread, __osExceptionPreamble, osSetIntMask) */
#define gl___osPiDevMgr        D_00000000 /* Data+0x2E430 Sym 2612: __osPiDevMgr (osCreatePiManager, osEPiStartDma, osPiGetCmdQueue) */
#define gl___osPiTable         D_00000000 /* Data+0x2E44C Sym 2577: __osPiTable (osCartRomInit, osDriveRomInit, osLeoDiskInit, osEPiLinkHandle) */
#define gl___osTimerList       D_00000000 /* Data+0x2E4B0 Sym 2636: __osTimerList (__osTimerServicesInit, __osInsertTimer, __osTimerInterrupt) */
#define gl___osViCurr          D_00000000 /* Data+0x2E520 Sym 2647: __osViCurr (__osViInit, __osViSwapContext, __osViGetCurrentContext) */
#define gl___osViNext          D_00000000 /* Data+0x2E524 Sym 2599: __osViNext (__osViInit, __osViSwapContext, osViSetXScale / Mode / Event) */
#define gl___osViDevMgr        D_00000000 /* Data+0x2E650 Sym 2672: __osViDevMgr (osCreateViManager) */
#define gl___osContPifRam      D_00000000 /* Bss+0x42770 Sym 2555: __osContPifRam (__osContGetInitData, __osPackRequestData, __osPackReadData) */
#define gl___osContLastCmd     D_00000000 /* Bss+0x427B0 Sym 2556: __osContLastCmd (u8; __osContRamRead / Write, __osPfsRequestData) */
#define gl___osMaxControllers  D_00000000 /* Bss+0x427B1 Sym 2557: __osMaxControllers (u8; osPfsIsPlug, __osPfsGetInitData, __osPfsRequestData, __osPackReadData) */
#define gl___osPfsPifRam       D_00000000 /* Bss+0x42CA0 Sym 2605: __osPfsPifRam (__osContRamRead / Write, __osPfsGetInitData / RequestData, osMotorStart / Stop; +0x3C = pifstatus) */
#define gl___osEventStateTab   D_00000000 /* Bss+0x43F30 Sym 2537, 128 bytes: __osEventStateTab (osSetEventMesg, __osExceptionPreamble; __osLeoResume reads +0x40 = [OS_EVENT_PI]) */
#define gl_DriveRomHandle      D_00000000 /* Bss+0x42820 Sym 2576: DriveRomHandle (OSPiHandle, osDriveRomInit) */
#define gl_CartRomHandle       D_00000000 /* Bss+0x43EB0 Sym 2616: CartRomHandle (OSPiHandle, osCartRomInit) */
#define gl_LeoDiskHandle       D_00000000 /* Bss+0x43FB0 Sym 2617: LeoDiskHandle (OSPiHandle, osLeoDiskInit) */
#define gl___osCurrentTime     D_00000000 /* Bss+0x44060 Sym 2632: __osCurrentTime (u64; __osTimerServicesInit, viMgrMain, osGetTime) */
#define gl___osTimerCounter    D_00000000 /* Bss+0x44070 Sym 2634: __osTimerCounter (__osSetTimerIntr, __osTimerInterrupt) */

/* ---- fs stream buffers (agent-a run 482) ---- */
#define gFsStream0             D_00000000 /* Bss+0x3F030 Sym 1520, 64 bytes: stream buffer of fs_bufWrite_334E8; region 0 of fs_piXfer_00065EE4 */
#define gFsStream1             D_00000000 /* Bss+0x3F070 Sym 1542, 64 bytes: stream buffer of fs_bufWrite_33BE4; region 1 of fs_piXfer_00065EE4 */
#define gFsStream2             D_00000000 /* Bss+0x3F0B0 Sym 1544, 64 bytes: stream buffer of fs_bufRead_33EB8; region 2 of fs_piXfer_00065EE4 */

/* ---- vtables (agent-a run 482) ----
 * Data objects whose span holds Text pointers in the Cfront layout (8-byte
 * entries, first function at +0xC, slot +0x1C = display, +0x24 = update,
 * +0x2C = handleMsg for Node classes). Named from the constructor that stores
 * its address (K = 0) into the vptr (this+0x28 for Node classes) LAST: a ctor
 * that stores two or three is the derived class running its inlined base
 * ctors first (BestScoreGadget_ctor stores TimeGadget_vtbl then BestGadget_vtbl;
 * QuitProcess_func_0000FC28 stores Process_vtbl, Process_vtbl_C3A0, QuitProcess_vtbl). A Data-offset suffix marks a
 * vtable whose class name is not certain. `scripts/uso-data-sites.py object
 * <Sym>` lists the slots; "Text ptrs in span" counts to the next named Data
 * object, so it can include a following unnamed table. */
#define TimeGadget_vtbl        D_00000000 /* Data+0xC77C Sym 861: vtable (10 Text ptrs in span, slot +0x1C TimeGadget_display_000140C4); vptr store by TimeGadget_ctor, BestTimeGadget_ctor, BestScoreGadget_ctor, PointsGadget_ctor, SpeedGadget_ctor, ... */
#define CountDownGadget_vtbl   D_00000000 /* Data+0xC7DC Sym 862: vtable (11 Text ptrs in span, slot +0x1C CountDownGadget_display); vptr store by CountDownGadget_ctor */
#define PointsGadget_vtbl_C854 D_00000000 /* Data+0xC854 Sym 863: vtable (10 Text ptrs in span, slot +0x1C PointsGadget_display_FFFFFFE0); vptr store by PointsGadget_ctor_00000ED0 */
#define BestGadget_vtbl        D_00000000 /* Data+0xC8B4 Sym 864: vtable (10 Text ptrs in span, slot +0x1C BestGadget_display_00014228); vptr store by BestTimeGadget_ctor, BestScoreGadget_ctor */
#define BoardsGadget_vtbl      D_00000000 /* Data+0xC914 Sym 865: vtable (10 Text ptrs in span, slot +0x1C BoardsGadget_display); vptr store by BoardsGadget_ctor */
#define CoolGadget_vtbl        D_00000000 /* Data+0xC974 Sym 866: vtable (10 Text ptrs in span, slot +0x1C CoolGadget_display); vptr store by CoolGadget_ctor */
#define CourseGadget_vtbl      D_00000000 /* Data+0xC9D4 Sym 867: vtable (11 Text ptrs in span, slot +0x1C CourseGadget_display); vptr store by CourseGadget_ctor */
#define PointsGadget_vtbl_CA4C D_00000000 /* Data+0xCA4C Sym 868: vtable (31 Text ptrs in span, slot +0x1C PointsGadget_display_000038F4); vptr store by PointsGadget_ctor_00003138 */
#define Process_vtbl           D_00000000 /* Data+0xC338 Sym 850: vtable (11 Text ptrs in span, slot +0x1C Node_display); vptr store by QuitProcess_func_0000FC28, QuitProcess_func_0000FD4C, PauseMenu_create, Fader_ctor, app_launchNameEnt */
#define QuitProcess_vtbl       D_00000000 /* Data+0xC2B8 Sym 849: vtable (14 Text ptrs in span, slot +0x1C Process_func_0000F76C); vptr store by QuitProcess_func_0000FC28, QuitProcess_func_0000FD4C */
#define Process_vtbl_C3A0      D_00000000 /* Data+0xC3A0 Sym 851: vtable (16 Text ptrs in span, slot +0x1C Process_func_0000F76C); vptr store by QuitProcess_func_0000FC28, QuitProcess_func_0000FD4C, app_launchNameEnt */
#define Fader_vtbl             D_00000000 /* Data+0xC6C8 Sym 853: vtable (12 Text ptrs in span, slot +0x1C Fader_func_0001304C); vptr store by Fader_ctor */
#define PauseMenu_vtbl         D_00000000 /* Data+0xC47C Sym 726: vtable (21 Text ptrs in span, slot +0x1C PauseMenu_func_00010540); vptr store by PauseMenu_create */
#define ColourItem_vtbl_75A0   D_00000000 /* Data+0x75A0 Sym 143: vtable (27 Text ptrs in span, slot +0x1C ColourItem_display); vptr store by func_00000C10, boot_ItemType_create_00002244, boot_ItemType_create_000046EC, boot_ItemType_create_00007B08, boot_ItemType_create_0000E690, ... */
#define SNodeItemType_vtbl     D_00000000 /* Data+0x1EFC0 Sym 414: vtable (16 Text ptrs in span, slot +0x1C Node_display); vptr store by item_registerTemplates, item_createBal, item_createBa2, item_createChk, item_createGtr, ... */
#define MNode_vtbl             D_00000000 /* Data+0x1EAA8 Sym 315: vtable (15 Text ptrs in span, slot +0x1C MNode_display); vptr store by cam_create, MapProp_ctor, MCamera_ctor, MNode_def, MNode_ctor, ... */
#define SnowBCollider_vtbl     D_00000000 /* Data+0x8698 Sym 493: vtable (13 Text ptrs in span, slot +0x1C QNode_display); vptr store by SnowBCollider_ctor */
#define BaseMat_vtbl           D_00000000 /* Data+0x20640 Sym 560: vtable (4 Text ptrs in span, slot +0x1C Mat_print); vptr store by SnowBoarder_loadBoardModel_0000B1B4, BaseMat_vslot4_000437C0, Mat_fromFrameBuffer_0004F85C, Mat_clone_0004FBA4 */
#define Mat_vtbl               D_00000000 /* Data+0x1FBEC Sym 562: vtable (8 Text ptrs in span, slot +0x1C Mat_print); vptr store by SnowBoarder_loadBoardModel_0000B1B4, BaseMat_vslot4_000437C0, Mat_fromFrameBuffer_0004F85C, Mat_clone_0004FBA4 */
#define LightTarget_vtbl       D_00000000 /* Data+0x207D8 Sym 312: vtable (11 Text ptrs in span, slot +0x1C LightTarget_display); vptr store by game_createLights, SnowBoarder_ctor, LightTarget_ctor */
#define SnowBoard_vtbl         D_00000000 /* Data+0x8BA8 Sym 609: vtable (slot +0x1C App_Display, +0x5C App_main: an App subclass); used by SnowBoard_func_0000E800, SnowBoard_func_0000E9FC */
#define Window_vtbl            D_00000000 /* Data+0x20508 Sym 2129: vtable (14 Text ptrs in span, slot +0x1C Window_display); vptr store by Window_ctor, Window_dtor */
#define BinFile_vtbl           D_00000000 /* Data+0x2B2B0 Sym 2475: vtable (5 Text ptrs in span, slot +0x1C BinFile_vfn2_00068340); vptr store by BinFileMgr_newFile_0006877C, ImageFileMgr_newFile_000688F8 */
#define ImageFile_vtbl         D_00000000 /* Data+0x2B278 Sym 2474: vtable (5 Text ptrs in span, slot +0x1C ImageFile_free_00068BAC); vptr store by ImageFileMgr_newFile_000688F8 */
#define BGBox_vtbl             D_00000000 /* Data+0x65E8 Sym 164: vtable (10 Text ptrs in span, slot +0x1C BGBox_func_00000D94); vptr store by BGBox_ctor */
#define U64BGWall_vtbl         D_00000000 /* Data+0x6648 Sym 165: vtable (32 Text ptrs in span, slot +0x1C U64BGWall_func_00001A44); vptr store by U64BGWall_ctor */
#define WorldRegistry_vtbl     D_00000000 /* Data+0x6DD0 Sym 176: vtable (31 Text ptrs in span, slot +0x1C Node_display); vptr store by WorldRegistry_create_00001E08 */
#define CamDirector_vtbl       D_00000000 /* Data+0x6FD8 Sym 247: vtable (20 Text ptrs in span, slot +0x1C Node_display); vptr store by CamDirector_ctor */
#define EnvSettings_vtbl       D_00000000 /* Data+0x76A8 Sym 348: vtable (20 Text ptrs in span, slot +0x1C EnvSettings_display); vptr store by EnvSettings_ctor */
#define Tv1_vtbl               D_00000000 /* Data+0x7818 Sym 350: vtable (slot +0x1C Tv1_display); vptr store by Tv1Tmpl_create */
#define MapProp_vtbl           D_00000000 /* Data+0x7C20 Sym 354: vtable (12 Text ptrs in span, slot +0x1C MapProp_display); vptr store by MapProp_ctor */
#define ShpNode_vtbl           D_00000000 /* Data+0x81E0 Sym 479: vtable (12 Text ptrs in span, slot +0x1C QNode_display); vptr store by ShpNode_ctor */
#define SnowBoarder_vtbl       D_00000000 /* Data+0x83D0 Sym 490: vtable (17 Text ptrs in span, slot +0x1C SnowBoarder_display); vptr store by SnowBoarder_ctor */
#define RaceTracker_vtbl       D_00000000 /* Data+0xD508 Sym 1125: vtable (34 Text ptrs in span, slot +0x1C RaceTracker_display); vptr store by RaceTracker_ctor */
#define MCamera_vtbl           D_00000000 /* Data+0x1E0E8 Sym 1496: vtable (14 Text ptrs in span, slot +0x1C MNode_display); vptr store by MCamera_ctor */
#define HostioDev_vtbl_1E1B8   D_00000000 /* Data+0x1E1B8 Sym 1502: vtable (5 Text ptrs in span, slot +0x1C hostio_devSlot2_000359B8); vptr store by app_func_000412E8 */
#define ConsoleDev_vtbl        D_00000000 /* Data+0x21DA0 Sym 1814: vtable (5 Text ptrs in span, slot +0x1C ConsoleDev_vfn2_00061B04); vptr store by app_func_000412E8 */
#define CamFollow_vtbl         D_00000000 /* Data+0x1E878 Sym 1636: vtable (10 Text ptrs in span, slot +0x1C Node_display); vptr store by CamFollow_ctor */
#define CamMan_vtbl            D_00000000 /* Data+0x1E8D8 Sym 1637: vtable (10 Text ptrs in span, slot +0x1C Node_display); vptr store by CamMan_ctor */
#define SeekNode_vtbl          D_00000000 /* Data+0x1E938 Sym 1638: vtable (11 Text ptrs in span, slot +0x1C Node_display); vptr store by SeekNode_ctor */
#define Node_vtbl              D_00000000 /* Data+0x1EA48 Sym 1667: vtable (10 Text ptrs in span, slot +0x1C Node_display); vptr store by Node_ctor */
#define SNode_vtbl             D_00000000 /* Data+0x1EB70 Sym 1668: vtable (10 Text ptrs in span, slot +0x1C SNode_display); vptr store by SNode_ctor */
#define CollBox_vtbl           D_00000000 /* Data+0x1ED58 Sym 1707: vtable (3 Text ptrs); vptr store by coll_boxCtor_0003CAA0 */
#define CollObjMgr_vtbl        D_00000000 /* Data+0x1ED98 Sym 1709: vtable (10 Text ptrs in span, slot +0x1C Node_display); vptr store by CollObjMgr_ctor */
#define ItemType_vtbl          D_00000000 /* Data+0x1EED8 Sym 1728: vtable (15 Text ptrs in span, slot +0x1C Node_display); vptr store by ItemType_ctor */
#define ColourItem_vtbl        D_00000000 /* Data+0x1EF60 Sym 1729: vtable (10 Text ptrs in span, slot +0x1C ColourItem_display); vptr store by ColourItem_ctor */
#define ItemMgr_vtbl           D_00000000 /* Data+0x1F198 Sym 1734: vtable (12 Text ptrs in span, slot +0x1C Node_display); vptr store by ItemMgr_ctor */
#define ColourItemType_vtbl    D_00000000 /* Data+0x1F228 Sym 1735: vtable (17 Text ptrs in span, slot +0x1C ColourItemType_display); vptr store by ColourItemType_ctor */
#define QNode_vtbl             D_00000000 /* Data+0x1F3F0 Sym 1790: vtable (14 Text ptrs in span, slot +0x1C QNode_display); vptr store by QNode_ctor */
#define App_vtbl               D_00000000 /* Data+0x1F4CC Sym 1801: vtable (18 Text ptrs in span, slot +0x1C App_Display); vptr store by App_ctor */
#define Node_vtbl_1F748        D_00000000 /* Data+0x1F748 Sym 1827: vtable (10 Text ptrs in span, slot +0x1C Node_display); vptr store by App_func_00041820 */
#define PolyShape_vtbl         D_00000000 /* Data+0x1F7D8 Sym 1845: vtable (13 Text ptrs in span, slot +0x1C PolyShape_display); vptr store by PolyShape_ctor */
#define MorphPoly_vtbl         D_00000000 /* Data+0x1F850 Sym 1846: vtable (13 Text ptrs in span, slot +0x1C MorphPoly_display); vptr store by MorphPoly_ctor */
#define ScreenWindow_vtbl      D_00000000 /* Data+0x1F8E0 Sym 1848: vtable (14 Text ptrs in span, slot +0x1C ScreenWindow_display); vptr store by ScreenWindow_ctor */
#define Sprite_vtbl            D_00000000 /* Data+0x1FB1C Sym 1871: vtable (11 Text ptrs in span, slot +0x1C Sprite_display_0004BAF4); vptr store by Sprite_ctor_0004B620 */
#define Shadow_vtbl            D_00000000 /* Data+0x1FB84 Sym 1872: vtable (11 Text ptrs in span, slot +0x1C Shadow_display_0004B2F4); vptr store by Shadow_ctor_0004B0A8 */
#define GfxObj_vtbl            D_00000000 /* Data+0x20400 Sym 2119: vtable (3 Text ptrs); vptr store by GfxObj_ctor_0004DF90 */
#define DisplayMgr_vtbl        D_00000000 /* Data+0x20448 Sym 2120: vtable (11 Text ptrs in span, slot +0x1C DisplayMgr_display); vptr store by DisplayMgr_ctor */
#define ShapeNode_vtbl         D_00000000 /* Data+0x20670 Sym 2148: vtable (11 Text ptrs in span, slot +0x1C Node_display); vptr store by ShapeNode_ctor_000521F8 */
#define BasePolyShape_vtbl     D_00000000 /* Data+0x206D8 Sym 2149: vtable (13 Text ptrs in span, slot +0x1C Node_display); vptr store by BasePolyShape_ctor */
#define BaseLight_vtbl         D_00000000 /* Data+0x20770 Sym 2150: vtable (11 Text ptrs in span, slot +0x1C MNode_display); vptr store by BaseLight_ctor */
#define CMod_vtbl              D_00000000 /* Data+0x20DA0 Sym 2178: vtable (13 Text ptrs in span, slot +0x1C CMod_display); vptr store by CMod_ctor */
#define MemTrackerMgr_vtbl     D_00000000 /* Data+0x21BD8 Sym 2307: vtable (10 Text ptrs in span, slot +0x1C Node_display); vptr store by MemTrackerMgr_ctor */
#define ConsoleMgr_vtbl        D_00000000 /* Data+0x21D28 Sym 2325: vtable (10 Text ptrs in span, slot +0x1C ConsoleMgr_display); vptr store by ConsoleMgr_ctor */
#define AnimPlayer_vtbl        D_00000000 /* Data+0x21F58 Sym 2364: vtable (12 Text ptrs in span, slot +0x1C Node_display); vptr store by AnimPlayer_ctor */
#define AnimDataMgr_vtbl       D_00000000 /* Data+0x21FD8 Sym 2365: vtable (13 Text ptrs in span, slot +0x1C Node_display); vptr store by AnimDataMgr_ctor */
#define DNode_vtbl             D_00000000 /* Data+0x22208 Sym 2402: vtable (14 Text ptrs in span, slot +0x1C QNode_display); vptr store by DNode_ctor */
#define PhysQNode_vtbl         D_00000000 /* Data+0x222A8 Sym 2403: vtable (12 Text ptrs in span, slot +0x1C QNode_display); vptr store by PhysQNode_ctor_00063884 */
#define Friction_vtbl          D_00000000 /* Data+0x22318 Sym 2404: vtable (12 Text ptrs in span, slot +0x1C QNode_display); vptr store by Friction_ctor_00063DC4 */
#define BinFileMgr_vtbl        D_00000000 /* Data+0x2B2E8 Sym 2476: vtable (11 Text ptrs in span, slot +0x1C Node_display); vptr store by BinFileMgr_ctor_000684AC */
#define ImageFileMgr_vtbl      D_00000000 /* Data+0x2B350 Sym 2477: vtable (11 Text ptrs in span, slot +0x1C Node_display); vptr store by ImageFileMgr_ctor */
#define SkinMgr_vtbl           D_00000000 /* Data+0x2C490 Sym 2501: vtable (11 Text ptrs in span, slot +0x1C SkinMgr_update); vptr store by SkinMgr_ctor */


/* ---- Bss / Data objects, wave 2 (agent-a run 491; `uso-data-sites.py object <Sym>`) ---- */
#define gNodeGlobals           D_00000000 /* Bss+0x3C058 Sym 550, 160 bytes = 40 Node pointers: Node_doCommand cmd 0x67 stores the node at [id], cmd 0x66 links a child under [id] (both check id < 40: "Global out of range %d", Data+0x1EC6C); SnowBoarder_ctor registers its sub-nodes; MCamera/BaseLight_doCommand forward to it */
#define gSnowBoard             D_00000000 /* Bss+0x58 Sym 326, 128 bytes: the static SnowBoard app object (SnowBoard_staticCtor_0000E9FC = its edg __sti: base ctor, then the vptr at +0x28); GameFlow_startMode / restart / loadProcUso and Process_exit (+0x30) use it */
#define gSysApi                D_00000000 /* Bss+0x416A0 Sym 1558, 4 bytes: POINTER to a system function table: +0x40 usoLoad(name) ("usoLoad() error loading file %s: %d", app_loadUso; "cant reload mainuso %d", boot_reloadMainUso), +0x44 unload(name), +0x4C remote openFile (fs_gdlOpenFile), +0x70 thread stack query (dbg_printThreads), +0x74 (app_loadUso); +0x0 / +0x4 = the memory range ("memory %x %x"). boot_main_00066850 installs game_libs entries: +0x34 mem_heapNewThunk, +0x80 dma_readChunked, +0x84 dbg_printf, +0x94 sys_timeInFrames, +0x98 / +0x9C heap_tempAlloc/FreeDefault; app_initSystem sets +0x8C / +0x90. Owner of the table itself not identified (kernel-side?) */
#define gBootBss               D_00000000 /* Bss+0x3F150 Sym 2422, 9552 bytes: the boot TU's static block (one exported Sym covers it): +0x0 segment base (sys_setSegBase/getSegBase), +0x1C0/+0x1C4 and +0x2370/+0x2374 the two 0x12345678 stack canaries (boot_checkStacks), +0x21C0 the boot thread (OSThread, boot_startThread), +0x2470 its stack / queue (boot_main) */
#define gSndCueQueues          D_00000000 /* Bss+0x338 Sym 1448, 352 bytes: the snd cue queues (snd_TriggerChannel8Cue/TickChannel8Fade/SetChannel8Mode at +0x0..+0x1C, snd_RequestCue +0x30, snd_RequestCue2 +0xF8, snd_SetPlayer2ChannelIO). Layout: audio.h SndCueQueues (view SND_CUEQ(gSndCueQueues); base-relocated sites use SND_CUEQ_AT_BASE) */
#define gPolyFlags_3EEC4       D_00000000 /* Bss+0x3EEC4 Sym 289, 4 bytes: render flag word (game_createMap sets/clears bits 0x8 / 0x100 per map pass; SnowBoarder_ctor |= 8); zeroed by the CMod static ctor (CMod_vfn0_0005165C). agent-b area: alias only */
#define gPolyGeomMode_3EEC8    D_00000000 /* Bss+0x3EEC8 Sym 290, 8 bytes: two mode words, [1] = geometry-mode-like bits (BasePolyShape_func_00052674 sets 1|0x2000|0x8000|0x20000, game_createMap 0xA8002 / |0x20000); [0] cleared with it. agent-b area: alias only */
#define gMainHeap              D_00000000 /* Bss+0x3C770 Sym 1797, 44 bytes: the default heap object: heap_printStats prints its free/largest and used/size (+0x28 = size; "%dk/%dk used/size"), app_setHeapMode / getHeapMode swap +0x20; heap_new, mem_delete, app_freeAllOfType use it */
#define gWindowDefaults        D_00000000 /* Bss+0x3EC20 Sym 324, 64 bytes: Window defaults template: Window_setDefaults copies +0x0/+0x8../+0x2C/+0x30 into each new Window; App_main fills it; game_create3DWindow / createProcessWindow read +0x30 */
#define gMemTrackerMgr         D_00000000 /* Bss+0x3EEF0 Sym 2306, POINTER to the MemTrackerMgr singleton: MemTrackerMgr_ctor stores `this` there; memtrack_begin / end / find walk its head / tail */
#define gl_osViModeTable       D_00000000 /* Data+0x2D700 Sym 1947, 3360 bytes = 42 x 0x50 OSViMode (type byte i at entry i, NTSC/PAL/MPAL x 14): game_libs' libultra osViModeTable; gfx_setVideoMode indexes it */

/* ---- Bss / Data objects, wave 3 (agent-a run 493) ---- */
#define gSysImStack            D_00000000 /* Bss+0x3EF10 Sym 2344, 256 bytes = 64 words: sys_imin pushes its argument at [depth] (and clears [depth + 1]; "too deep" print at depth >= 63), sys_imout walks it up to the first 0 and releases each entry, sys_stackPeek_0006170C reads [depth - 1] */
#define gSysImDepth            D_00000000 /* Data+0x21DE8 Sym 2345: the gSysImStack depth / count (sys_imin increments, sys_imout's --count < 0 test, sys_stackPeek_0006170C) */
#define gGfxFbWidth            D_00000000 /* Data+0x1F974 Sym 1853: framebuffer width; gfx_clearFbRect_00043468 uses it as the row stride in halfwords (y * w + x), gfx_setScreenSize(w, h) stores it with Sym 1854; GfxLib_init / reset */
#define gGfxFbHeight           D_00000000 /* Data+0x1F978 Sym 1854: framebuffer height (gfx_setScreenSize second argument; GfxLib_init / reset, gfx_drawFbSplash_00048184) */
#define gSysDevBlock_3A4E8     D_00000000 /* Bss+0x3A4E8 Sym 1505, 5112 bytes (inferred span): the pad / device block of RcpMgr_service, pad_initGlobal, pad_requestGlobal, pad_setSlotValueGlobal, pad_isSlotState2Global, pad_decodeButtonMap_00034CC8, sys_pollDevices_00034B64, sys_fatalHaltLoop. Kind only */

/* ---- string / path tables (DataRelocs point at RoData strings; agent-a run 491) ---- */
#define gBoardModelUsoPaths    D_00000000 /* Data+0x8338 Sym 486, 9 char*: "rom/tahoe151.uso", "rom/merlot147.uso", ... the board model USOs (SnowBoarder_loadBoardModel / createBoardShape) */
#define gBoardBinPaths         D_00000000 /* Data+0x8360 Sym 487, 10 char*: "rom/board1.bin", ... (SnowBoarder_func_0000CAE8) */
#define gPlayerBinPaths        D_00000000 /* Data+0x8388 Sym 488, 8 char*: "rom/player1.bin" .. (SnowBoarder_func_0000CAE8) */
#define gBoarderUsoPaths       D_00000000 /* Data+0x83AC Sym 489, 8 char*: "rom/boarder1.uso" .. (SnowBoarder_loadBoardModel) */
#define gProcUsoPaths          D_00000000 /* Data+0xC510 Sym 727, 7 char*: "rom/genproc.uso", "rom/nameent.uso", "rom/gamegfx.uso", "rom/charsel.uso", "rom/charsps.uso", "rom/charfir.uso", "rom/charmap.uso" (app_launchGenProc, app_launchNameEnt, app_createGame_*) */
#define gGameModeNames         D_00000000 /* Data+0xC450 Sym 852, 10 char*: "N64DEMO", "TITLEDEMO", "HEAD2HEAD", "MULTIGAMERACE", ... (GameFlow_startMode) */
#define gPlaceNames            D_00000000 /* Data+0xCF58 Sym 952, 3 char*: "1st", "2nd", "3rd" (no direct site) */
#define gFaultFlagNames        D_00000000 /* Data+0x21130 Sym 2229, 82 char*: "BD", "IP8", "IP7", ... Cause / Status bit names (fault_printFlags, fault_printThread, ProcNodeList) */

/* ---- wave 4 (agent-a run 498) ---- */
#define gCourseColl          D_00000000 /* Bss+0x50 Sym 451, 4 bytes: pointer to the course collision object. game_createMap stores the object it builds from the map's morphpolyshape; coll_pushClear_00035E64 / gfx_screenRaycast / SnowBoarder_procColl / SnowBCollider_onContact / DNode_solveContacts raycast against it (null = no course loaded) */
#define gTriEdgeNext         D_00000000 /* Data+0x1ED90 Sym 1708, s16[3] = {1, 2, 0} (+pad): edge k of a collision triangle runs vi[k] -> vi[gTriEdgeNext[k]] (CollShape_height / below / raycast / query, game_libs_ncc_3B2EC.cpp D_3B2EC_c) */
#define gMorphPolyShade      D_00000000 /* Bss+0x3C8E4 Sym 1925, s16[3] (+pad): the three shade values MorphPoly_shade applies per vertex when self+0x38 & 0x8000 (MorphPoly_vslot12_0004AFB4, PolyShape_shade_00049DBC) */
#define gProcNodeList_41700  D_00000000 /* Bss+0x41700 Sym 557, 80 bytes: ProcNodeList's state block (clears +0..+0xC on entry); its +0 list head is walked by SnowBoarder_ctor and the skin_cmdSkinMgr / SkinAnim / SkinShape node-list commands. Role inferred */
#define gConsoleNode_3EF04   D_00000000 /* Bss+0x3EF04 Sym 805, 4 bytes: pointer to a Node (vptr +0x28, display slot +0x1C) created in App_main; every TextWin_ctor registers its window with it and the fatal-error paths (sys_fatalSwapBuffer_00034180, sys_fatalHaltLoop) display it directly. Role inferred */
#define gNodeDbgOut_3C140    D_00000000 /* Bss+0x3C140 Sym 1675, 16 bytes: first argument of the debug-print calls in the MNode / QNode display / update debug-mode switch (root+0xB0). Role inferred */

/* ---- class-name strings passed to the Node base ctor (Data-base sites; agent-a run 491) ---- */
#define sTimeGadgetName        GL_DATA(0xCAFC)  /* "TimeGadget" (TimeGadget_ctor) */
#define sBestTimeGadgetName    GL_DATA(0xCB20)  /* "BestTimeGadget" */
#define sBestScoreGadgetName   GL_DATA(0xCB48)  /* "BestScoreGadget" */
#define sCountDownGadgetName   GL_DATA(0xCB8C)  /* "CountDownGadget" */
#define sPointsGadgetName      GL_DATA(0xCBC8)  /* "PointsGadget" (PointsGadget_ctor) */
#define sPointsGadgetName_CBE8 GL_DATA(0xCBE8)  /* "PointsGadget" (PointsGadget_ctor_00000ED0) */
#define sSpeedGadgetName       GL_DATA(0xCC04)  /* "SpeedGadget" */
#define sCourseGadgetName      GL_DATA(0xCCBC)  /* "CourseGadget" */
#define sPointsGadgetName_CCD8 GL_DATA(0xCCD8)  /* "PointsGadget" (PointsGadget_ctor_00003138) */
#define sSNodeName             GL_DATA(0x1ECF4) /* "Snode" (SNode_ctor) */
#define sNullCollShapeName     GL_DATA(0x1EE7C) /* "NullCollShape" */
#define sCollObjMgrName        GL_DATA(0x1EEAC) /* "CollObjMgr" */
#define sMNodeName             GL_DATA(0x1F2E4) /* "<MNode>" (MNode_ctor) */
#define sCModName              GL_DATA(0x2107C) /* "CMod" */
#define sMemTrackerMgrName     GL_DATA(0x21CE8) /* "MemTrackerMgr" */
#define sMemTrackTotal         GL_DATA(0x21CF8) /* "Total": MemTrackerMgr_ctor's first tracker */
#define sMemTrackBootUp        GL_DATA(0x21D00) /* "BootUp": its second tracker */
#define sConsoleMgrName        GL_DATA(0x21D88) /* "ConsoleMgr" */
#define sAnimDataMgrName       GL_DATA(0x220DC) /* "AnimDataMgr" (agent-b area: not converted) */
#define sImageFileMgrName      GL_DATA(0x2B420) /* "ImageFileMgr" */

#endif
