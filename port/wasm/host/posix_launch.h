/* port/wasm/host/posix_launch.c: the double-click start-up (plan 30) */
#ifndef PORT_WASM_POSIX_LAUNCH_H
#define PORT_WASM_POSIX_LAUNCH_H
char **port_posix_launch(int argc, char **argv, int *argcOut);
int port_posix_gui(void);
const char *port_posix_data_dir(void);
void port_posix_alert(const char *text);
#endif
