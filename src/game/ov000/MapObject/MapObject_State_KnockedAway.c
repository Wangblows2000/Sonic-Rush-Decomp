/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapObject_SpawnDestroyBurst();
s32 MapObject_SpawnFreedAnimal();

void MapObject_State_KnockedAway(u8 *p0) {
    *(u8 *)((u8 *)p0 + 0x1) = (*(u8 *)((u8 *)p0 + 0x1) + 0x10);
    s32 t0 = *(u32 *)((u8 *)p0 + 0xc);
    if (t0 == 0) {
        MapObject_SpawnDestroyBurst(p0, t0);
        MapObject_SpawnFreedAnimal(p0);
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
}
