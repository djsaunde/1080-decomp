/* port/win32: the Windows build's start-up (win32_host.c; plan section 27). */
#ifndef PORT_WIN32_HOST_H
#define PORT_WIN32_HOST_H

/* before the ROM is read: the timer period, the log, the ROM / save paths when not given, PORT_NATIVE=1
 * unless --headless, real time when started without arguments */
void port_win32_setup(int argc, char **argv, const char **romPath, int romGiven, const char **sramPath,
                      int sramGiven, int *realtime);
/* a message box when the program has no console (a fatal error after a double-click) */
void port_win32_message(const char *title, const char *text);

#endif
