#include <nitro/types.h>

typedef struct UnkStruct_022c6db4_3 {
    u8 pad[0x18];
    void *unk18;
    u8 pad1[0x2c - 0x1c];
} UnkStruct_022c6db4_3;

typedef struct UnkStruct_022c6db4 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x1b8 - 0x58];
    void *unk1B8;
    u8 pad2[0x20c - 0x1bc];
    UnkStruct_022c6db4_3 unk20C[4];
} UnkStruct_022c6db4;

typedef struct UnkStruct_022c6db4_2 {
    u8 pad[0x10];
    UnkStruct_022c6db4 *unk10;
} UnkStruct_022c6db4_2;

extern UnkStruct_022c6db4_2 *gTaskCurrent;

void StageObj_HandleCollider(UnkStruct_022c6db4 *o, UnkStruct_022c6db4_3 *c);
void ObjCollisionMap_Register(void *p);

void Boss1Hammer_Collide(void) {
    UnkStruct_022c6db4 *o = gTaskCurrent->unk10;
    if (o->unk54 & 0xc)
        return;
    if (o->unk20C[0].unk18 != NULL)
        StageObj_HandleCollider(o, &o->unk20C[0]);
    if (o->unk20C[1].unk18 != NULL)
        StageObj_HandleCollider(o, &o->unk20C[1]);
    if (o->unk20C[2].unk18 != NULL)
        StageObj_HandleCollider(o, &o->unk20C[2]);
    if (o->unk20C[3].unk18 != NULL)
        StageObj_HandleCollider(o, &o->unk20C[3]);
    if (o->unk1B8 != NULL)
        ObjCollisionMap_Register(&o->unk1B8);
}
