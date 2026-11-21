#include <nitro/types.h>

typedef struct {
    u8 pad[0x2c];
    u32 flags;
    u8 pad2[0x18];
} UnkStruct_022e855c;

typedef struct {
    u8 pad[0x9a8];
    s32 unk9a8;
    UnkStruct_022e855c objs[21];
    u8 padf94[0x70];
    UnkStruct_022e855c objs3[3];
    UnkStruct_022e855c objs2[8];
    u8 pad3[4];
} UnkStruct_022e4eb8;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void ZoneMap_InitNodesAndPaths(UnkStruct_022e4eb8 *p, s32 a);
void ZoneMap_InitSprite(UnkStruct_022e855c *obj, s32 a, u16 id, s32 b, s32 c);

#pragma thumb on
void ZoneMap_InitNodeLayer(UnkStruct_022e4eb8 *p, s32 a, s32 b, s32 c) {
    vs32 zero2;
    vu32 zero;
    s32 i;
    UnkStruct_022e855c *obj;

    zero = 0;
    MIi_CpuClear32(zero, p, sizeof(UnkStruct_022e4eb8));
    p->unk9a8 = a;
    ZoneMap_InitNodesAndPaths(p, b);
    ZoneMap_InitSprite(&p->objs[0], c, 0, 0, 4);
    ZoneMap_InitSprite(&p->objs[1], c, 1, 0, 4);
    ZoneMap_InitSprite(&p->objs[2], c, 2, 0, 4);
    ZoneMap_InitSprite(&p->objs[3], c, 3, 0, 4);
    ZoneMap_InitSprite(&p->objs[4], c, 4, 0, 4);
    ZoneMap_InitSprite(&p->objs[5], c, 5, 0, 4);
    ZoneMap_InitSprite(&p->objs[6], c, 6, 0, 4);
    ZoneMap_InitSprite(&p->objs[7], c, 7, 0, 4);
    ZoneMap_InitSprite(&p->objs[8], c, 8, 0, 4);
    ZoneMap_InitSprite(&p->objs[9], c, 9, 0, 4);
    ZoneMap_InitSprite(&p->objs[10], c, 10, 0, 4);
    ZoneMap_InitSprite(&p->objs[11], c, 11, 0, 4);
    ZoneMap_InitSprite(&p->objs[12], c, 12, 0, 4);
    ZoneMap_InitSprite(&p->objs[13], c, 13, 0, 4);
    ZoneMap_InitSprite(&p->objs[14], c, 14, 0, 4);
    ZoneMap_InitSprite(&p->objs[15], c, 15, 0, 4);
    ZoneMap_InitSprite(&p->objs[16], c, 16, 0, 4);
    ZoneMap_InitSprite(&p->objs[18], c, 17, 0, 4);
    ZoneMap_InitSprite(&p->objs[17], c, 18, 0, 4);
    ZoneMap_InitSprite(&p->objs[19], c, 19, 0, 4);
    ZoneMap_InitSprite(&p->objs[20], c, 31, 0, 2);
    i = 0;
    obj = p->objs2;
    zero2 = 0;
    for (; i < 8; i++) {
        ZoneMap_InitSprite(obj, c, i + 0x17, zero2, 2);
        obj++;
    }
    obj = &p->objs3[0];
    ZoneMap_InitSprite(obj, c, 20, 0, 2);
    obj->flags &= ~0x60000;
    obj->flags |= 0x40000;
    obj = &p->objs3[1];
    ZoneMap_InitSprite(obj, c, 21, 0, 2);
    obj->flags &= ~0x60000;
    obj->flags |= 0x40000;
    obj = &p->objs3[2];
    ZoneMap_InitSprite(obj, c, 22, 0, 2);
    obj->flags &= ~0x60000;
    obj->flags |= 0x40000;
}
#pragma thumb reset
