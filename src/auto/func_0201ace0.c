/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_02080f60[];
s32 func_0201acc0();
s32 OS_Terminate();

void func_0201ace0(s32 p0, s32 p1) {
    if (((s32)p1 & 0x3f) == 0x11) {
        s32 t0 = *(u32 *)data_02080f60;
        s32 v1 = 0;
        if (t0 != 0) {
            s32 t2 = ((s32 (*)())t0)(v1, t0);
            v1 = t2;
        }
        if (v1 == 0) {
            return;
        }
        func_0201acc0();
        return;
    }
    OS_Terminate();
}
