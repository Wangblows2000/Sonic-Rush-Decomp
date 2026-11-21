/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 StageObj_CollideMapHorizontal();
s32 StageObj_CollideMapVertical();

void StageObj_CollideMapImpl(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x400000);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) & 1);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x400000);
    }
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0xf);
    *(u16 *)((u8 *)p0 + 0x5e) = 0;
    s32 t1 = *(s16 *)((u8 *)p0 + 0x28);
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x400) == 0) {
        StageObj_CollideMapHorizontal(p0, 0, t1);
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x800) == 0) {
        StageObj_CollideMapVertical(p0);
    }
    s32 t2 = (*(u32 *)((u8 *)p0 + 0x58) & 1);
    if (t2 != 0) {
        *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)p0 + 0x14) & ~0xff);
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 0x400) != 0) {
        return;
    }
    StageObj_CollideMapHorizontal(p0, 1, t1);
}
