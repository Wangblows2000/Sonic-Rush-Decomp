/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 WH_SetError();
extern u8 sConnectMode[];
s32 WH_CallbackForWFS();

void WH_StateOutSetMPData(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x2);
    if (t0 != 0) {
        if (t0 != 0xf) {
            WH_SetError(t0);
            return;
        }
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x20);
    if (t1 != 0) {
        ((s32 (*)())t1)((t0 == 0), t1);
    }
    if ((u32)(*(u32 *)sConnectMode - 0x6) > (u32)1) {
        return;
    }
    WH_CallbackForWFS(p0);
}
