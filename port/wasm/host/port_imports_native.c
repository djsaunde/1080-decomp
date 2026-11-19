/* port/wasm/host/port_imports_native.c -- the "port" imports of the wasm32 game: the in-process host of plan 21.9
 * (port/gfx/port_native.h, port/gfx/fast3d/native_host.cpp), compiled with PORT_WASM_HOST so that it reads the
 * game's addresses in the linear memory (plan 30). */
#include "wasm_host.h"
#include "../../gfx/port_native.h"

u32 w2c_port_port_native_open(struct w2c_port *p) {
    (void)p;
    return port_native_open();
}
u32 w2c_port_port_native_task(struct w2c_port *p, u32 root, u32 rq, u32 imgs, u32 max) {
    (void)p;
    return (u32)port_native_task(root, rq, imgs, (int)max);
}
void w2c_port_port_native_invalidate(struct w2c_port *p, u32 addr, u32 size) {
    (void)p;
    port_native_invalidate(addr, size);
}
void w2c_port_port_native_black(struct w2c_port *p, u32 black) {
    (void)p;
    port_native_black((int)black);
}
u32 w2c_port_port_native_depth(struct w2c_port *p, u32 img, u32 idx) {
    (void)p;
    return (u32)port_native_depth(img, idx);
}
void w2c_port_port_native_frame(struct w2c_port *p, u32 step) {
    (void)p;
    port_native_frame(step);
}
u32 w2c_port_port_native_idle(struct w2c_port *p, u32 us) {
    (void)p;
    return (u32)port_native_idle(us);
}
u32 w2c_port_port_native_poll(struct w2c_port *p) {
    (void)p;
    return (u32)port_native_poll();
}
void w2c_port_port_native_close(struct w2c_port *p) {
    (void)p;
    port_native_close();
}
