/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gDisplayTable[];
extern u8 gGameState[];
extern u8 gCamera[];
s32 ZoneFx_InitBrightnessBlend();

void Stage_InitBlend(void) {
    s32 t0 = *(u32 *)gDisplayTable;
    *(u16 *)((u8 *)t0 + 0x20) = 0;
    *(u16 *)((u8 *)t0 + 0x20) = ((*(u16 *)((u8 *)t0 + 0x20) & ~0xc0) | 0x40);
    *(u16 *)((u8 *)t0 + 0x20) = (*(u16 *)((u8 *)t0 + 0x20) | 0x100);
    *(u16 *)((u8 *)t0 + 0x20) = (*(u16 *)((u8 *)t0 + 0x20) | 0x200);
    *(u16 *)((u8 *)t0 + 0x20) = (*(u16 *)((u8 *)t0 + 0x20) | 0x400);
    s32 t1 = (*(u16 *)((u8 *)t0 + 0x20) | 0x800);
    *(u16 *)((u8 *)t0 + 0x20) = t1;
    s32 t2 = *(u32 *)gGameState;
    s32 v3 = t2;
    if (t2 != 0) {
        s32 v4 = v3;
        if (v3 == 1) {
            *(u16 *)((u8 *)t0 + 0x22) = 0x709;
            v4 = 0x709;
        }
        v3 = v4;
    } else {
        *(u16 *)((u8 *)t0 + 0x22) = 0xe0c;
        v3 = 0xe0c;
    }
    s32 t5 = (*(u32 *)((u8 *)gCamera + 0xc8) & 0x100000);
    if (t5 == 0) {
        return;
    }
    ZoneFx_InitBrightnessBlend(t5, v3, t1);
}
