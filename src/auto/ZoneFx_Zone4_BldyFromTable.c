/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gZoneFxZone4BldyFlashTable[];
extern u8 gDisplayTable[];

s32 ZoneFx_Zone4_BldyFromTable(s32 p0, s32 p1) {
    s16 t0 = *(s16 *)((u8 *)p0 + ((s32)p1 << 1));
    u16 t1 = *(u16 *)((u8 *)((s32)p0 + ((s32)p1 << 1)) + 0x4);
    *(u16 *)((u8 *)p0 + ((s32)p1 << 1)) = (t0 + 0x1);
    u16 t2 = *(u16 *)((u8 *)p0 + ((s32)p1 << 1));
    u8 t3 = *(u8 *)((u8 *)gZoneFxZone4BldyFlashTable + (t2 - 0x1));
    u32 t4 = *(u32 *)((u8 *)gDisplayTable + ((s32)p1 << 2));
    *(u16 *)((u8 *)t4 + 0x24) = t3;
    return ((u32)t2 >= (u32)t1);
}
