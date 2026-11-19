1080 Snowboarding - PC port, Android build (arm64; Android 8.0 or later, OpenGL ES 3.0)
====================================================================================

No game data is included. You need your own 1080 Snowboarding (USA, 1.0) cartridge dump (.z64, .v64 or .n64).
Made for handhelds with built-in gamepad controls (tested target: the AYN Thor); any Android gamepad works.

Install (once)
  Either:
    - copy 1080-snowboarding.apk to the device (USB, a cloud drive, an SD card), open it in a file manager and
      allow "Install unknown apps" for that file manager when Android asks; or
    - from a PC with adb (USB debugging on):   adb install -r 1080-snowboarding.apk
  Updates install over the old version (adb install -r, or open the new APK) as long as they are signed with the
  same key; a CI build (signed with a throwaway key) needs the old app uninstalled first (that deletes the save).

Give it the ROM (once)
  Start the app. With no ROM it asks for one: Choose ROM, pick your dump in the file picker. It is copied into the
  app, so the original can be moved or deleted afterwards. Or, before starting the app, copy the dump to
      Android/data/dev.djsaunde.tenshoe/files/baserom.z64
  (internal storage; a PC over USB or adb can write there:
      adb push baserom.z64 /sdcard/Android/data/dev.djsaunde.tenshoe/files/baserom.z64
   the folder exists after the app's first start). A ROM there is preferred over the copied one. A file that is
  not 1080 Snowboarding is refused and the picker asked for again.

Controls (the built-in gamepad; rebindable in the settings menu, page P1)
  Left stick        stick (the right stick = C buttons)
  A                 A                        B / X      B
  Y                 C-up                     LB / LT    Z trigger
  RB / RT           R                        Select     L
  Start             START                    D-pad      D-pad
  Select + Start    the settings menu (graphics, audio, controls); Back (the system key or gesture) too
  In the menu: D-pad / stick move, A changes / activates, B or Start closes it, LB / RB change the page,
  X adds a second binding, Y clears one. If A and B are swapped, swap the face buttons in the device's controller
  settings (Xbox / Nintendo layout) or rebind A and B on page P1.

Where things are
  The save (1080.sram), the settings (1080.ini) and the log (1080.log) are in the app's private storage:
      /data/user/0/dev.djsaunde.tenshoe/files/
  A file manager cannot open that folder; adb can:
      adb shell run-as dev.djsaunde.tenshoe cat files/1080.log > 1080.log
  (run-as needs a debuggable build; with this release build use logcat instead, which gets every log line:)
      adb logcat -s tenshoe
  Clearing the app's storage (Settings > Apps > 1080 Snowboarding > Storage) deletes the save and the copied ROM.

Notes
  - Sound: SDL's AAudio / OpenSL ES output with a 100 ms buffer; raise it in the menu (Audio > Buffer) if it
    crackles, lower it for less delay.
  - Graphics: OpenGL ES 3.0 (libultraship's Fast3D). MSAA is not available on ES; the internal resolution, widescreen
    and frame-rate settings work as on the PC.
  - The second screen of dual-screen devices is not used yet.
  - The game runs as WebAssembly translated to C (wasm2c), compiled natively for arm64, as the macOS build.
