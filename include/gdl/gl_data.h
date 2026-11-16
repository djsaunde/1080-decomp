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
#define gHostio       D_00000000 /* Bss+0x3C768 Sym 338, 8 bytes: the HostIO device object (HostioDev, gdl/hostio.h: flags, connected), first argument of every hostio_beginGroup/add/endGroup call in the 47 <Class>_hostioParms functions */
#define gWindowDefaults D_00000000 /* Bss+0x3EC20 Sym 324, 0x40 bytes: the values Window_setDefaults copies into every new Window (+0x8 width, +0xC height, +0x10 / +0x14 origin, +0x18 near, +0x1C far, +0x28 flags, +0x2C projType, +0x30 clearColour; window.h WindowDefaults). App_main fills it; game_create3DWindow / createProcessWindow read +0x30 */
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
#define gl___osContPifRam      D_00000000 /* Bss+0x42770 Sym 2555: __osContPifRam (gl___osContGetInitData_6BC44, gl___osPackRequestData_6BD14, gl___osPackReadData, gl_osContGetReadData) */
#define gl___osContLastCmd     D_00000000 /* Bss+0x427B0 Sym 2556: __osContLastCmd (u8; __osContRamRead / Write, __osPfsRequestData) */
#define gl___osMaxControllers  D_00000000 /* Bss+0x427B1 Sym 2557: __osMaxControllers (u8; osPfsIsPlug, __osPfsGetInitData, __osPfsRequestData, gl___osPackRequestData_6BD14) */
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
#define ColourItem_vtbl_75A0   D_00000000 /* Data+0x75A0 Sym 143: vtable (27 Text ptrs in span, slot +0x1C ColourItem_display); vptr store by ColourItem_create_00000C10, boot_ItemType_create_00002244, boot_ItemType_create_000046EC, boot_ItemType_create_00007B08, boot_ItemType_create_0000E690, ... */
#define SNodeItemType_vtbl     D_00000000 /* Data+0x1EFC0 Sym 414: vtable (16 Text ptrs in span, slot +0x1C Node_display); vptr store by item_registerTemplates, item_createBal, item_createBa2, item_createChk, item_createGtr, ... */
#define MNode_vtbl             D_00000000 /* Data+0x1EAA8 Sym 315: vtable (15 Text ptrs in span, slot +0x1C MNode_display); vptr store by cam_create, MapProp_ctor, MCamera_ctor, MNode_def, MNode_ctor, ... */
#define SnowBCollider_vtbl     D_00000000 /* Data+0x8698 Sym 493: vtable (13 Text ptrs in span, slot +0x1C QNode_display); vptr store by SnowBCollider_ctor */
#define BaseMat_vtbl           D_00000000 /* Data+0x20640 Sym 560: vtable (4 Text ptrs in span, slot +0x1C Mat_print); vptr store by SnowBoarder_loadBoardModel_0000B1B4, Mat_draw, Mat_fromFrameBuffer_0004F85C, Mat_clone_0004FBA4 */
#define Mat_vtbl               D_00000000 /* Data+0x1FBEC Sym 562: vtable (8 Text ptrs in span, slot +0x1C Mat_print); vptr store by SnowBoarder_loadBoardModel_0000B1B4, Mat_draw, Mat_fromFrameBuffer_0004F85C, Mat_clone_0004FBA4 */
#define LightTarget_vtbl       D_00000000 /* Data+0x207D8 Sym 312: vtable (11 Text ptrs in span, slot +0x1C LightTarget_display); vptr store by game_createLights, SnowBoarder_ctor, LightTarget_ctor */
#define SnowBoard_vtbl         D_00000000 /* Data+0x8BA8 Sym 609: vtable (slot +0x1C App_Display, +0x5C App_main: an App subclass); used by SnowBoard_func_0000E800, SnowBoard_func_0000E9FC */
#define Window_vtbl            D_00000000 /* Data+0x20508 Sym 2129: vtable (14 Text ptrs in span, slot +0x1C Window_display); vptr store by Window_ctor, Window_dtor */
#define BinFile_vtbl           D_00000000 /* Data+0x2B2B0 Sym 2475: vtable (5 Text ptrs in span, slot +0x1C BinFile_release); vptr store by BinFileMgr_newFile_0006877C, ImageFileMgr_newFile_000688F8 */
#define ImageFile_vtbl         D_00000000 /* Data+0x2B278 Sym 2474: vtable (5 Text ptrs in span, slot +0x1C ImageFile_free_00068BAC); vptr store by ImageFileMgr_newFile_000688F8 */
#define BGBox_vtbl             D_00000000 /* Data+0x65E8 Sym 164: vtable (10 Text ptrs in span, slot +0x1C BGBox_display); vptr store by BGBox_ctor */
#define U64BGWall_vtbl         D_00000000 /* Data+0x6648 Sym 165: vtable (32 Text ptrs in span, slot +0x1C U64BGWall_display); vptr store by U64BGWall_ctor */
#define WorldRegistry_vtbl     D_00000000 /* Data+0x6DD0 Sym 176: vtable (31 Text ptrs in span, slot +0x1C Node_display); vptr store by WorldRegistry_create_00001E08 */
#define CamDirector_vtbl       D_00000000 /* Data+0x6FD8 Sym 247: vtable (20 Text ptrs in span, slot +0x1C Node_display); vptr store by CamDirector_ctor */
#define EnvSettings_vtbl       D_00000000 /* Data+0x76A8 Sym 348: vtable (20 Text ptrs in span, slot +0x1C EnvSettings_display); vptr store by EnvSettings_ctor */
#define Tv1_vtbl               D_00000000 /* Data+0x7818 Sym 350: vtable (slot +0x1C Tv1_display); vptr store by Tv1Tmpl_create */
#define MapProp_vtbl           D_00000000 /* Data+0x7C20 Sym 354: vtable (12 Text ptrs in span, slot +0x1C MapProp_display); vptr store by MapProp_ctor */
#define ShpNode_vtbl           D_00000000 /* Data+0x81E0 Sym 479: vtable (12 Text ptrs in span, slot +0x1C QNode_display); vptr store by ShpNode_ctor */
#define SnowBoarder_vtbl       D_00000000 /* Data+0x83D0 Sym 490: vtable (17 Text ptrs in span, slot +0x1C SnowBoarder_display); vptr store by SnowBoarder_ctor */
#define RaceTracker_vtbl       D_00000000 /* Data+0xD508 Sym 1125: vtable (34 Text ptrs in span, slot +0x1C RaceTracker_display); vptr store by RaceTracker_ctor */
#define MCamera_vtbl           D_00000000 /* Data+0x1E0E8 Sym 1496: vtable (14 Text ptrs in span, slot +0x1C MNode_display); vptr store by MCamera_ctor */
#define SysLib_vtbl_1E1B8      D_00000000 /* Data+0x1E1B8 Sym 1502: gSysLib's own (most-derived) vtable, stored at gSysLib +0x18 by app_staticInit_000412E8 after the base SysLibBase_vtbl (slots hostio_devWrite / hostio_devRead / hostio_devSlot2 / app_loadUso; not gHostio's: HostioDev has no vptr) */
#define SysLibBase_vtbl        D_00000000 /* Data+0x21DA0 Sym 1814: gSysLib's BASE-class vtable (5 Text ptrs in span; slots 0..3 SysLibBase_devWrite / devRead / devSlot2 / loadUso stubs, overridden in SysLib_vtbl_1E1B8; slot +0x1C SysLibBase_devSlot2_00061B04); vptr store at gSysLib +0x18 by the base ctor in app_staticInit_000412E8 (symbol SysLibBase_vtbl_412E8_j) */
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
#define gPolyFlags_3EEC4       D_00000000 /* Bss+0x3EEC4 Sym 289, 4 bytes: render flag word (game_createMap sets/clears bits 0x8 / 0x100 per map pass; SnowBoarder_ctor |= 8); zeroed by the CMod static ctor (CMod_staticInit). agent-b area: alias only */
#define gPolyGeomMode_3EEC8    D_00000000 /* Bss+0x3EEC8 Sym 290, 8 bytes: two mode words, [1] = geometry-mode-like bits (BasePolyShape_initGeomGlobals_00052674 sets 1|0x2000|0x8000|0x20000, game_createMap 0xA8002 / |0x20000); [0] cleared with it. agent-b area: alias only */
#define gMainHeap              D_00000000 /* Bss+0x3C770 Sym 1797, 44 bytes: the default heap object: heap_printStats prints its free/largest and used/size (+0x28 = size; "%dk/%dk used/size"), app_setHeapMode / getHeapMode swap +0x20; heap_new, mem_delete, app_freeAllOfType use it */
#define gMemTrackerMgr         D_00000000 /* Bss+0x3EEF0 Sym 2306, POINTER to the MemTrackerMgr singleton: MemTrackerMgr_ctor stores `this` there; memtrack_begin / end / find walk its head / tail */
#define gl_osViModeTable       D_00000000 /* Data+0x2D700 Sym 1947, 3360 bytes = 42 x 0x50 OSViMode (type byte i at entry i, NTSC/PAL/MPAL x 14): game_libs' libultra osViModeTable; gfx_setVideoMode indexes it */

/* ---- Bss / Data objects, wave 3 (agent-a run 493) ---- */
#define gSysImStack            D_00000000 /* Bss+0x3EF10 Sym 2344, 256 bytes = 64 words: sys_imin pushes its argument at [depth] (and clears [depth + 1]; "too deep" print at depth >= 63), sys_imout walks it up to the first 0 and releases each entry, sys_stackPeek_0006170C reads [depth - 1] */
#define gSysImDepth            D_00000000 /* Data+0x21DE8 Sym 2345: the gSysImStack depth / count (sys_imin increments, sys_imout's --count < 0 test, sys_stackPeek_0006170C) */
#define gGfxFbWidth            D_00000000 /* Data+0x1F974 Sym 1853: framebuffer width; gfx_clearFbRect_00043468 uses it as the row stride in halfwords (y * w + x), gfx_setScreenSize(w, h) stores it with Sym 1854; GfxLib_init / reset */
#define gGfxFbHeight           D_00000000 /* Data+0x1F978 Sym 1854: framebuffer height (gfx_setScreenSize second argument; GfxLib_init / reset, gfx_drawFbSplash_00048184) */
#define gSysDevBlock_3A4E8     D_00000000 /* Bss+0x3A4E8 Sym 1505, 5112 bytes (inferred span): the pad / device block of RcpMgr_service, pad_initGlobal, pad_requestGlobal, pad_setSlotValueGlobal, pad_hasRumblePakGlobal, pad_decodeButtonMap_00034CC8, sys_pollDevices_00034B64, sys_fatalHaltLoop. Kind only */

/* ---- string / path tables (DataRelocs point at RoData strings; agent-a run 491) ---- */
#define gBoardModelUsoPaths    D_00000000 /* Data+0x8338 Sym 486, 9 char*: "rom/tahoe151.uso", "rom/merlot147.uso", ... the board model USOs (SnowBoarder_loadBoardModel / createBoardShape) */
#define gBoardBinPaths         D_00000000 /* Data+0x8360 Sym 487, 10 char*: "rom/board1.bin", ... (SnowBoarder_reset) */
#define gPlayerBinPaths        D_00000000 /* Data+0x8388 Sym 488, 8 char*: "rom/player1.bin" .. (SnowBoarder_reset) */
#define gBoarderUsoPaths       D_00000000 /* Data+0x83AC Sym 489, 8 char*: "rom/boarder1.uso" .. (SnowBoarder_loadBoardModel) */
#define gProcUsoPaths          D_00000000 /* Data+0xC510 Sym 727, 7 char*: "rom/genproc.uso", "rom/nameent.uso", "rom/gamegfx.uso", "rom/charsel.uso", "rom/charsps.uso", "rom/charfir.uso", "rom/charmap.uso" (app_launchGenProc, app_launchNameEnt, app_createGame_*) */
#define gGameModeNames         D_00000000 /* Data+0xC450 Sym 852, 10 char*: "N64DEMO", "TITLEDEMO", "HEAD2HEAD", "MULTIGAMERACE", ... (GameFlow_startMode) */
#define gPlaceNames            D_00000000 /* Data+0xCF58 Sym 952, 3 char*: "1st", "2nd", "3rd" (no direct site) */
#define gFaultFlagNames        D_00000000 /* Data+0x21130 Sym 2229, 82 char*: "BD", "IP8", "IP7", ... Cause / Status bit names (fault_printFlags, fault_printThread, ProcNodeList) */

/* ---- wave 4 (agent-a run 498) ---- */
#define gCourseColl          D_00000000 /* Bss+0x50 Sym 451, 4 bytes: pointer to the course collision object. game_createMap stores the object it builds from the map's morphpolyshape; coll_pushClear_00035E64 / gfx_screenRaycast / SnowBoarder_procColl / SnowBCollider_onContact / DNode_solveContacts raycast against it (null = no course loaded) */
#define gTriEdgeNext         D_00000000 /* Data+0x1ED90 Sym 1708, s16[3] = {1, 2, 0} (+pad): edge k of a collision triangle runs vi[k] -> vi[gTriEdgeNext[k]] (CollShape_height / below / raycast / query, game_libs_ncc_3B2EC.cpp gTriEdgeNext_3B2EC_c) */
#define gMorphPolyShade      D_00000000 /* Bss+0x3C8E4 Sym 1925, s16[3] (+pad): the three shade values MorphPoly_shade applies per vertex when self+0x38 & 0x8000 (MorphPoly_makeEdgeVtxs, PolyShape_shade_00049DBC) */
#define gSkinMgrs_41700      D_00000000 /* Bss+0x41700 Sym 557, 80 bytes: the SkinMgr table, SkinMgr *[MAX_SKIN_MGRS = 4] indexed by the skin slot. skin_cmdSkinMgr stores the new SkinMgr at [slot]; skin_cmdSkinAnim / skin_cmdSkinShape read [slot] ("No skinmgr for skinanim"); ProcNodeList (0x5591C) clears the 4 slots (gSkinMgrs_41700_5591C_z*); SnowBoarder_ctor copies slots 0 / 2 / 1 (gSkinMgrs_41700_90cc_fz). agent-b run 553 (was gProcNodeList_41700) */
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

/* ---- RoData constant pools (agent-a run 504; READABILITY_PLAN 9.1.9) ----
 * kRo_<Func> = the RoData-section constants that <Func> reads (non-string RoData-base sites, K =
 * RoData offset), for every function that reads two or more. Defined as GL_RODATA(first K), so
 * `kRo_<Func>` is the pool's address; a per-site alias of one constant is the linker symbol
 * `kRo_<Func>_<K>` (pure renames). Values are decoded by the load at the site: f = lwc1 float,
 * d = ldc1 double, jtbl = a switch jump table (lw of a Text offset), table = address taken (addiu),
 * bu = lbu. `uso-data-sites.py check` verifies each range against the relocs and each
 * kRo_<Func>_<K> site (RoData base, that K, inside <Func>). */
#define kRo_snd_updatePlayer                       GL_RODATA(0x58) /* RoData+0x58..0x60, 2 K: 58=5000d 60=0.3d */
#define kRo_game_createLights                      GL_RODATA(0x524) /* RoData+0x524..0x52C, 3 K: 524=0.4f 528=1e+06f 52C=1e+07f */
#define kRo_cam_create                             GL_RODATA(0x530) /* RoData+0x530..0x534, 2 K: 530=-1500f 534=-1500f */
#define kRo_game_create3DWindow                    GL_RODATA(0x538) /* RoData+0x538..0x540, 3 K: 538=0.239216f 53C=0.560784f 540=0.701961f */
#define kRo_game_setupDemoRace                     GL_RODATA(0x544) /* RoData+0x544..0x548, 2 K: 544=2.99f 548=1.99f */
#define kRo_game_setupDemoSplit                    GL_RODATA(0x54C) /* RoData+0x54C..0x550, 2 K: 54C=2.99f 550=1.99f */
#define kRo_RiderCam_update_00004948               GL_RODATA(0x554) /* RoData+0x554..0x558, 2 K: 554=3.14159f 558=3.14159f */
#define kRo_item_registerTemplates                 GL_RODATA(0x598) /* RoData+0x598..0x5AC, 6 K: 598=0.2f 59C=0.17f 5A0=0.17f 5A4=0.17f 5A8=0.17f 5AC=0.17f */
#define kRo_SnowBoarder_ctor                       GL_RODATA(0x868) /* RoData+0x868..0x8BC, 22 K: 868=0.4f 86C=0.21f 870=1.244f 874=0.212f 878=0.168f 87C=0.972f 880=0.798f 884=0.03f 888=0.16f 88C=0.2f 890=0.2f 894=0.06f ... (10 more) */
#define kRo_SnowBoarder_updateOrient               GL_RODATA(0x8C8) /* RoData+0x8C8..0x910, 10 K: 8C8=0.15d 8D0=-0.04d 8D8=0.4d 8E0=-0.2f 8E8=0.015d 8F0=0.2f 8F8=-0.1d 900=0.8d 908=0.8d 910=0.724f */
#define kRo_SnowBoarder_updateDyn                  GL_RODATA(0x914) /* RoData+0x914..0x918, 2 K: 914=0.1f 918=0.1d */
#define kRo_SnowBoarder_procColl                   GL_RODATA(0x920) /* RoData+0x920..0x950, 11 K: 920=100000f 924=335f 928=-335f 930=0.6d 938=0.08f 93C=0.184f 940=0.024f 944=0.064f 948=0.008f 94C=0.4f 950=0.2d */
#define kRo_SnowBoarder_createColliders_0000CCE0   GL_RODATA(0x95C) /* RoData+0x95C..0x96C, 5 K: 95C=-335f 960=-335f 964=-335f 968=-335f 96C=-335f */
#define kRo_SnowBoarder_display                    GL_RODATA(0x970) /* RoData+0x970..0x978, 2 K: 970=0.3f 978=-0.2d */
#define kRo_SnowBoarder_handleMsg                  GL_RODATA(0x988) /* RoData+0x988..0x990, 2 K: 988=0.1d 990=0.2d */
#define kRo_gui_centerRows_0001016C                GL_RODATA(0xC10) /* RoData+0xC10..0xC18, 3 K: C10=0.9f C14=0.9f C18=0.9f */
#define kRo_Fader_display                          GL_RODATA(0xC50) /* RoData+0xC50..0xC60, 5 K: C50=0.02f C54=0.2f C58=0.4f C5C=0.1f C60=0.4f */
#define kRo_CoolGadget_ctor                        GL_RODATA(0xC7C) /* RoData+0xC7C..0xC94, 7 K: C7C=0.756863f C80=0.482353f C84=0.964706f C88=0.0352941f C8C=0.607843f C90=0.466667f C94=0.18f */
#define kRo_CourseGadget_display                   GL_RODATA(0xCB8) /* RoData+0xCB8..0xCC4, 4 K: CB8=0.784314f CBC=0.117647f CC0=0.364706f CC4=0.635294f */
#define kRo_PointsGadget_update_00003430           GL_RODATA(0xCC8) /* RoData+0xCC8..0xCF4, 5 K: CC8=0.1f CCC=jtbl CEC=0.1f CF0=0.1f CF4=0.1f */
#define kRo_ColourParms_ctor                       GL_RODATA(0xD0C) /* RoData+0xD0C..0xDE4, 53 K: D0C=0.196078f D10=0.313726f D14=0.27451f D18=0.196078f D1C=0.313726f D20=0.27451f D24=0.196078f D28=0.313726f D2C=0.27451f D30=0.196078f D34=0.313726f D38=0.27451f ... (41 more) */
#define kRo_RaceProcess_handleInput                GL_RODATA(0xE04) /* RoData+0xE04..0xE40, 8 K: E04=jtbl E1C=jtbl E2C=0.1f E30=0.1f E34=0.05f E38=0.01f E3C=0.1f E40=jtbl */
#define kRo_AudioHeap_InitAdsrDecayTable           GL_RODATA(0xE70) /* RoData+0xE70..0xE74, 2 K: E70=0.33f E74=0.66f */
#define kRo_AudioLoad_Init                         GL_RODATA(0xEC8) /* RoData+0xEC8..0xED0, 3 K: EC8=20.0304f ECC=16.546f ED0=16.713f */
#define kRo_AudioThread_ProcessGlobalCmd           GL_RODATA(0xEE0) /* RoData+0xEE0..0xF10, 2 K: EE0=jtbl F10=jtbl */
#define kRo_Audio_InitSampleState                  GL_RODATA(0xFC0) /* RoData+0xFC0..0xFC4, 2 K: FC0=0.707f FC4=4096f */
#define kRo_Audio_NoteSetResamplingRate            GL_RODATA(0xFC8) /* RoData+0xFC8..0xFD0, 3 K: FC8=1.99998f FCC=3.99996f FD0=1.99998f */
#define kRo_Audio_BuildSyntheticWave               GL_RODATA(0xFD4) /* RoData+0xFD4..0xFEC, 7 K: FD4=0.99999f FD8=1.0465f FDC=1.99999f FE0=0.52325f FE4=3.99999f FE8=0.26263f FEC=0.13081f */
#define kRo_Audio_AdsrUpdate                       GL_RODATA(0xFF0) /* RoData+0xFF0..0x1018, 3 K: FF0=jtbl 1014=32767f 1018=1e-05f */
#define kRo_AudioSeq_SeqLayerProcessScriptStep2    GL_RODATA(0x1058) /* RoData+0x1058..0x105C, 2 K: 1058=16129f 105C=jtbl */
#define kRo_AudioSeq_SequenceChannelProcessScript  GL_RODATA(0x10B0) /* RoData+0x10B0..0x125C, 3 K: 10B0=jtbl 11B8=jtbl 125C=jtbl */
#define kRo_AudioSeq_SequencePlayerProcessSequence GL_RODATA(0x13E0) /* RoData+0x13E0..0x1494, 2 K: 13E0=jtbl 1494=jtbl */
#define kRo_snd_SoundObjUpdate                     GL_RODATA(0x16E0) /* RoData+0x16E0..0x172C, 6 K: 16E0=0.6f 16E4=jtbl 16F8=jtbl 1724=1.6f 1728=1.6f 172C=0.8f */
#define kRo_snd_SoundObjUpdateChannel              GL_RODATA(0x1730) /* RoData+0x1730..0x173C, 4 K: 1730=1.3f 1734=0.85f 1738=0.8f 173C=0.7f */
#define kRo_snd_SoundObjSetAngle                   GL_RODATA(0x1740) /* RoData+0x1740..0x174C, 4 K: 1740=0.6f 1744=0.4f 1748=0.8f 174C=1.2f */
#define kRo_snd_dispatchEvent                  GL_RODATA(0x1750) /* RoData+0x1750..0x17A4, 2 K: 1750=jtbl 17A4=jtbl */
#define kRo_snd_SetChannel8Mode                    GL_RODATA(0x1868) /* RoData+0x1868..0x1870, 3 K: 1868=0.01f 186C=0.1f 1870=0.004f */
#define kRo_snd_HandleSeqEvent_30AF4               GL_RODATA(0x1874) /* RoData+0x1874..0x18E0, 4 K: 1874=jtbl 18AC=0.1f 18B0=jtbl 18E0=jtbl */
#define kRo_snd_HandleSeqEnd                       GL_RODATA(0x1900) /* RoData+0x1900..0x193C, 2 K: 1900=jtbl 193C=jtbl */
#define kRo_snd_UpdateChannel10Emitter             GL_RODATA(0x1968) /* RoData+0x1968..0x1974, 4 K: 1968=0.1f 196C=0.98f 1970=0.1f 1974=0.8f */
#define kRo_StickCtrlA_update_00037348             GL_RODATA(0x19F0) /* RoData+0x19F0..0x19F4, 2 K: 19F0=0.06f 19F4=0.65f */
#define kRo_StickCtrlB_update_00037540             GL_RODATA(0x19F8) /* RoData+0x19F8..0x1A00, 3 K: 19F8=0.06f 19FC=0.65f 1A00=0.9f */
#define kRo_MNode_doCommand                        GL_RODATA(0x1A10) /* RoData+0x1A10..0x1A7C, 3 K: 1A10=0.01f 1A64=jtbl 1A7C=3.05185e-05f */
#define kRo_CollShape_raycast                      GL_RODATA(0x1AA8) /* RoData+0x1AA8..0x1AB0, 2 K: 1AA8=1e-05d 1AB0=-0.0001d */
#define kRo_ColourItemType_findNextByZ        GL_RODATA(0x1AC4) /* RoData+0x1AC4..0x1AC8, 2 K: 1AC4=-1e+07f 1AC8=1e+07f */
#define kRo_SNodeItemType_findNextByZ         GL_RODATA(0x1AD0) /* RoData+0x1AD0..0x1ADC, 4 K: 1AD0=1e+07f 1AD4=-1e+07f 1AD8=-1e+07f 1ADC=1e+07f */
#define kRo_QNode_doCommand                        GL_RODATA(0x1AE0) /* RoData+0x1AE0..0x1AFC, 3 K: 1AE0=0.01f 1AE4=jtbl 1AFC=3.05185e-05f */
#define kRo_gfx_emitProjection                     GL_RODATA(0x1B60) /* RoData+0x1B60..0x1B68, 3 K: 1B60=-32700f 1B64=32700f 1B68=32700f */
#define kRo_Window_setDefaults                     GL_RODATA(0x1B70) /* RoData+0x1B70..0x1B80, 3 K: 1B70=2200f 1B74=3000f 1B80=0.484f */
#define kRo_gfx_screenRaycast                      GL_RODATA(0x1BA8) /* RoData+0x1BA8..0x1BAC, 2 K: 1BA8=-100000f 1BAC=100000f */
#define kRo_BasePolyShape_parse                    GL_RODATA(0x1BE4) /* RoData+0x1BE4..0x1C78, 2 K: 1BE4=jtbl 1C78=10000f */
#define kRo_light_setShadowMatrix_00054D04         GL_RODATA(0x1C80) /* RoData+0x1C80..0x1C88, 2 K: 1C80=0.02d 1C88=100000f */
#define kRo_math_quatSlerp                         GL_RODATA(0x2038) /* RoData+0x2038..0x2040, 2 K: 2038=1e-06d 2040=3.14159f */
#define kRo_math_lookAtF                           GL_RODATA(0x205C) /* RoData+0x205C..0x2060, 2 K: 205C=10000f 2060=1e-07f */
#define kRo_Friction_applyContact                GL_RODATA(0x20A0) /* RoData+0x20A0..0x20B8, 4 K: 20A0=0.001d 20A8=0.22d 20B0=0.2d 20B8=0.2f */
#define kRo_DNodeAccum_apply_00064388              GL_RODATA(0x20C8) /* RoData+0x20C8..0x20D0, 2 K: 20C8=1e-05d 20D0=455f */
#define kRo_DNode_ctor                             GL_RODATA(0x20D4) /* RoData+0x20D4..0x20E0, 4 K: 20D4=0.2f 20D8=-0.72f 20DC=0.216f 20E0=0.2f */
#define kRo_math_acosf                             GL_RODATA(0x2268) /* RoData+0x2268..0x2280, 5 K: 2268=1.001d 2270=-1.001d 2278=41720f 227C=41720f 2280=3.14159d */
#define kRo_fsin                                   GL_RODATA(0x2310) /* RoData+0x2310..0x2350, 5 K: 2310=table 2338=0.31831d 2340=3.14159d 2348=3.17865e-08d 2350=0f */
#define kRo_gl___osExceptionPreamble               GL_RODATA(0x2360) /* RoData+0x2360..0x2380, 2 K: 2360=bu:0x141818 2380=jtbl */
#define kRo_fcos                                   GL_RODATA(0x24E0) /* RoData+0x24E0..0x2520, 5 K: 24E0=table 2508=0.31831d 2510=3.14159d 2518=3.17865e-08d 2520=0f */
#define kRo_gl_Ldtob                               GL_RODATA(0x2540) /* RoData+0x2540..0x2598, 3 K: 2540=table 2588=table 2598=1e+08d */

/* ---- named per-site offsets (agent-b run 591) ----
 * The K of a `(char *)&<per-site placeholder> + K` site whose placeholder is its own zero symbol
 * (the distinct symbol is the matched CSE / reloc lever, so GL_DATA / GL_RODATA / GL_BSS cannot
 * spell it). kRoF_ = a RoData float, kBss_ = a Bss word, kTag_ = a 4-byte parm tag string in Data.
 * Offsets resolved by `uso-data-sites.py sites <Func>` (section base + K). */
#define kRoF_SNode_radiusMax_1A80   0x1A80  /* RoData+0x1A80: f32 30000 (SNode_hostioParms: the "Radius" slider max; placeholder D_39C8C_max) */
#define kBss_memTrace_3C160         0x3C160 /* Bss+0x3C160 (inside Sym 1761 at Bss+0x3C154, +0xC; the object is unnamed): a word; mem_delete and heap_new log while it is nonzero, app_setCurrentHeap stores it, App_main clears it (placeholder D_415A4_z) */
#define kTag_col_2108C              0x2108C /* Data+0x2108C: "col" (BaseLight_ctor: the colour parm's GdlParmTag word; placeholder sTag_col_2108C) */

#endif
