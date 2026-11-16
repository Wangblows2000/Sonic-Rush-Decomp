/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
s32 func_ov000_0233ae0c();
extern u8 data_ov000_02364d90[];

void func_ov000_0233bf8c(void) {
    s32 t0 = *(u8 *)((u8 *)gGameState + 0x1c);
    s32 v1 = 0;
    if (t0 == 8) {
        s32 v2 = v1;
        if (*(u8 *)((u8 *)gGameState + 0x1d) == 2) {
            v2 = 1;
        }
        v1 = v2;
    }
    if (v1 != 0) {
        return;
    }
    if ((u32)t0 < (u32)9) {
        s32 t3 = *(u8 *)((u8 *)gGameState + 0x1d);
        if ((u32)t3 < (u32)2) {
            func_ov000_0233ae0c(t3, t0, v1);
            return;
        }
    }
    ((s32 (*)())*(u32 *)((u8 *)data_ov000_02364d90 + (t0 << 2)))(*(u32 *)((u8 *)data_ov000_02364d90 + (t0 << 2)));
}
