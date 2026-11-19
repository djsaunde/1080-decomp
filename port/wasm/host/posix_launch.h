/* port/wasm/host/posix_launch.c: the double-click start-up (plan 30) */
#ifndef PORT_WASM_POSIX_LAUNCH_H
#define PORT_WASM_POSIX_LAUNCH_H
char **port_posix_launch(int argc, char **argv, int *argcOut);
int port_posix_gui(void);
const char *port_posix_data_dir(void);
void port_posix_alert(const char *text);
/* a host that finds the ROM and the data directory itself (Android, plan 31) and passes --rom: the double-click
 * behaviour anyway (a fatal exit names the log and shows the alert through hook) */
void port_posix_set_gui(const char *dataDir, void (*alertHook)(const char *text));
#endif
