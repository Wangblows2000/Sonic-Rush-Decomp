/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02068ce4();
void func_ov000_022fea7c();

void func_ov000_022fbbb8(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x88) != 0) {
        s32 t0 = func_02068ce4(*(s16 *)((u8 *)p0 + 0x44), 0x100, 2, 0, 8);
        *(u16 *)((u8 *)p0 + 0x44) = t0;
        s32 t1 = func_02068ce4(*(s16 *)((u8 *)p0 + 0x46), 0x100, 2, 0, 8);
        *(u16 *)((u8 *)p0 + 0x46) = t1;
        if (*(s16 *)((u8 *)p0 + 0x44) == 0x100) {
            if (*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x68) != (s32)(u32)func_ov000_022fea7c) {
                *(u32 *)((u8 *)p0 + 0x88) = 0;
            }
        }
    }
    if (*(u32 *)((u8 *)p0 + 0x88) != 0) {
        return;
    }
    s32 t2 = func_02068ce4(*(s16 *)((u8 *)p0 + 0x44), 0, 2, 0, 8);
    *(u16 *)((u8 *)p0 + 0x44) = t2;
    s32 t3 = func_02068ce4(*(s16 *)((u8 *)p0 + 0x46), 0x200, 2, 0, 8);
    *(u16 *)((u8 *)p0 + 0x46) = t3;
    s32 t4 = *(s16 *)((u8 *)p0 + 0x44);
    if (t4 == 0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}
