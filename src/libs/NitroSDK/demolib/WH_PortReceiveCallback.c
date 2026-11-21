/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WH_SetError();
extern u8 data_022c1970[];

void WH_PortReceiveCallback(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        WH_SetError(t0, p0);
        return;
    }
    s32 t1 = *(u32 *)data_022c1970;
    if (t1 == 0) {
        return;
    }
    s32 t2 = *(u16 *)((u8 *)p0 + 0x4);
    if (t2 == 0x15) {
        ((s32 (*)())t1)(*(u16 *)((u8 *)p0 + 0x12), *(u32 *)((u8 *)p0 + 0xc), *(u16 *)((u8 *)p0 + 0x10), t1);
        return;
    }
    if (t2 != 9) {
        return;
    }
    ((s32 (*)())t1)(*(u16 *)((u8 *)p0 + 0x12), 0, 0);
}
