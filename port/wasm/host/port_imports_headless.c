/* port/wasm/host/port_imports_headless.c -- the "port" imports of the wasm32 game without a window (plan 30):
 * the headless runs (the gate's state.log and gfx-dump comparisons) never set PORT_NATIVE, so the game
 * never calls them; each answers "no window" if it does. */
#include "wasm_host.h"

u32 w2c_port_port_native_open(struct w2c_port *p) {
    (void)p;
    return 0;
}
u32 w2c_port_port_native_task(struct w2c_port *p, u32 root, u32 rq, u32 imgs, u32 max) {
    (void)p, (void)root, (void)rq, (void)imgs, (void)max;
    return 0;
}
void w2c_port_port_native_invalidate(struct w2c_port *p, u32 addr, u32 size) {
    (void)p, (void)addr, (void)size;
}
void w2c_port_port_native_black(struct w2c_port *p, u32 black) {
    (void)p, (void)black;
}
u32 w2c_port_port_native_depth(struct w2c_port *p, u32 img, u32 idx) {
    (void)p, (void)img, (void)idx;
    return (u32)-1;
}
void w2c_port_port_native_frame(struct w2c_port *p, u32 step) {
    (void)p, (void)step;
}
u32 w2c_port_port_native_idle(struct w2c_port *p, u32 us) {
    (void)p, (void)us;
    return 0;
}
u32 w2c_port_port_native_poll(struct w2c_port *p) {
    (void)p;
    return 0;
}
void w2c_port_port_native_close(struct w2c_port *p) {
    (void)p;
}
void w2c_port_port_native_flow(struct w2c_port *p, u32 flow) {
    (void)p, (void)flow;
}
