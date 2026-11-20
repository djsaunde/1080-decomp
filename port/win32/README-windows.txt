1080 Snowboarding - PC port, Windows build (32-bit, runs on any 64-bit Windows 10 / 11)
=====================================================================================

No game data is included. You need your own 1080 Snowboarding (USA, 1.0) cartridge dump.

Running
  1. Put tenshoe.exe in a folder of its own.
  2. Put your ROM next to it as baserom.z64 (.n64 / .v64 dumps work too; if it is the only ROM in the
     folder, any name works). Or just start tenshoe.exe: it asks for the ROM with a file dialog and
     remembers the choice (rom-path.txt).
  3. Double-click tenshoe.exe.
  The save (1080.sram), the settings (1080.ini) and the log (1080.log) go next to the exe, or to
  %APPDATA%\1080-pc when that folder is not writable.

Keys (rebindable: F1)
  Stick       arrow keys or W A S D (Shift = half tilt)
  A / B       X or Space / Z
  Z trigger   C
  L / R       Q / E
  START       Enter
  C buttons   I J K L (up left down right)
  D-pad       T F G H (up left down right)
  F1 settings menu   F11 or Alt+Enter fullscreen   Esc quit
  F3 perf log on / off: once a second a "perf:" line goes to 1080.log (where the time goes, for a bug
     report about slow or choppy play; [debug] perf = 1 in 1080.ini turns it on from the start)

Gamepads (XInput pads and the DirectInput pads SDL knows; up to 4)
  Left stick = stick, A = A, B / X = B, Y = C-up, LB / LT = Z, RB / RT = R, Back = L, Start = START,
  right stick = C buttons, D-pad = D-pad. Gamepad 1 is controller 1; more pads and players in F1.

Command line (optional)
  tenshoe.exe --rom PATH --sram PATH --realtime      play with explicit paths
  tenshoe.exe --headless --frames N --trace LOG      the test modes of the Linux build
