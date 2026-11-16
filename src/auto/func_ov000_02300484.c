/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
extern u8 data_ov000_0236ac60[];
s32 Sound_SetPositional();
s32 func_ov000_022ffa58();
extern u8 gSaveData[];
extern u8 data_022c4560[];
s32 func_ov000_023008a0();
s32 func_ov000_02300808();

void func_ov000_02300484(u8 *p0) {
    if (((s32)*(s16 *)((u8 *)p0 + 0xa2) >> 4) == 0x16) {
        if ((*(u32 *)((u8 *)p0 + 0x1f4) & 1) == 0) {
            Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x77);
            Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
            func_ov000_022ffa58(p0, 0, 0);
        }
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 1);
    }
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x8) = 0x78;
    if ((*(u32 *)((u8 *)gSaveData + 0x8) & 1) != 0) {
        s32 t0 = *(u32 *)((u8 *)data_022c4560 + 0x10);
        if (t0 != 2) {
            if (t0 != 3) {
                *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) + ((u32)*(u32 *)((u8 *)p0 + 0x8) >> 1));
            }
        }
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x1e0);
    if (*(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) == 0) {
        if (*(u8 *)((u8 *)t1 + 0x8) == 0) {
            func_ov000_023008a0(p0, t1);
            return;
        }
    }
    func_ov000_02300808(p0);
}
