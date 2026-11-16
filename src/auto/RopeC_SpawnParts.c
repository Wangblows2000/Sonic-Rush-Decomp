/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapObject_Spawn();

void RopeC_SpawnParts(u8 *p0) {
    s32 t0 = MapObject_Spawn(0xe0, *(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14), 0, *(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6), 0, 0, 0, 0);
    if (t0 != 0) {
        *(u32 *)((u8 *)t0 + 0x88) = (u32)p0;
    }
    s32 t1 = MapObject_Spawn(0xe1, (*(u32 *)((u8 *)p0 + 0x10) - 0x9000), (*(u32 *)((u8 *)p0 + 0x14) + 0x4000), 0, 0, 0, 0, 0, 0);
    if (t1 != 0) {
        *(u32 *)((u8 *)t1 + 0x88) = (u32)p0;
    }
    s32 t2 = MapObject_Spawn(0xe2, *(u32 *)((u8 *)p0 + 0x10), (*(u32 *)((u8 *)p0 + 0x14) + 0x1800), 0, 0, 0, 0, 0, 0);
    if (t2 != 0) {
        *(u32 *)((u8 *)t2 + 0x88) = (u32)p0;
    }
}
