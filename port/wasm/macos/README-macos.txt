1080 Snowboarding - PC port, macOS build (Apple Silicon; macOS 11 or later)
==========================================================================

No game data is included. You need your own 1080 Snowboarding (USA, 1.0) cartridge dump.

First start
  1. Unzip 1080-pc-macos.zip. Move 1080.app wherever you like (Applications, or a folder of its own).
  2. The app is not signed with an Apple Developer ID, so macOS blocks the first double-click
     ("cannot be opened because the developer cannot be verified" / "is damaged"). Either:
       - right-click (or Control-click) 1080.app, choose Open, then Open again in the dialog; or
       - in Terminal:  xattr -dr com.apple.quarantine /path/to/1080.app
     You only do this once.
  3. Give it your ROM, one of:
       - put it next to 1080.app (in the same folder) as baserom.z64 (.n64 / .v64 dumps work too; if it is the
         only ROM in that folder, any name works);
       - or put it in ~/Library/Application Support/1080-pc/ (same rules);
       - or just open the app: it asks for the ROM with a file dialog and remembers the choice (rom-path.txt).
  The save (1080.sram), the settings (1080.ini) and the log (1080.log) go to
  ~/Library/Application Support/1080-pc/  (Finder: Go > Go to Folder..., paste that path).

Keys (rebindable: F1; on a MacBook, F1 / F11 may need the fn key)
  Stick       arrow keys or W A S D (Shift = half tilt)
  A / B       X or Space / Z
  Z trigger   C
  L / R       Q / E
  START       Enter
  C buttons   I J K L (up left down right)
  D-pad       T F G H (up left down right)
  F1 settings menu   F11 fullscreen   Esc quit

Gamepads (the controllers SDL knows: Xbox, PlayStation, Switch Pro, MFi ...; up to 4)
  Left stick = stick, A = A, B / X = B, Y = C-up, LB / LT = Z, RB / RT = R, Back = L, Start = START,
  right stick = C buttons, D-pad = D-pad. Gamepad 1 is controller 1; more pads and players in F1.

Command line (optional; Terminal)
  1080.app/Contents/MacOS/1080 --rom PATH --sram PATH --realtime    play with explicit paths (set PORT_NATIVE=1
                                                                      for the window)
  1080.app/Contents/MacOS/1080 --rom PATH --frames N --trace LOG    the headless test mode of the Linux build

How it works: the game is compiled to WebAssembly (a 32-bit data model, as on the N64) and translated to C with
wasm2c, then built natively for arm64. Rendering is OpenGL 4.1 (libultraship's Fast3D), sound and input SDL2.
