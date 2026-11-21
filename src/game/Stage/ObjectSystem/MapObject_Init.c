#include <game/Stage/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
} UnkStruct_Prm;

extern u8 data_ov000_023686e0[];

void MapObject_In_Default(void);
void MapObject_SpriteCallback_Default(void);
void MapObject_Collide_Default(void);
void MapObject_OnHit_Default(void);
void ObjRect_SetAtkDefFlags(UnkStruct_Coll *c, s32 a, s32 b);

void MapObject_Init(void *w, s32 a, s32 x, s32 y) {
    UnkStruct_StageObj *s = w;
    UnkStruct_Prm *prm = (UnkStruct_Prm *)a;

    *(void **)((u8 *)s + 0x6c) = MapObject_In_Default;
    s->unk54 |= 0x40;
    *(void **)((u8 *)s + 0x74) = MapObject_SpriteCallback_Default;
    s->unk78 = MapObject_Collide_Default;
    s->unk1E8 = x >> 8;
    s->unk1EC = y >> 8;
    s->unk10 = x;
    s->unk14 = y;
    if (prm->unk2 < 0x40)
        s->unk0 = 2;
    else
        s->unk0 = 3;
    s->unk40 = 0x2a;
    s->unk42 = 0xf00;
    s->unk54 |= 1;
    s->unk5C |= 1;
    s->unk58 |= 0x200;
    s->coll0.unk28 = 0x12;
    s->coll1.unk28 = 0x12;
    s->coll2.unk28 = 0x12;
    s->coll0.unk1C = MapObject_OnHit_Default;
    ObjRect_SetAtkDefFlags(&s->coll0, 9, 0xa);
    ObjRect_SetAtkDefFlags(&s->coll1, 0x13, 0x13);
    ObjRect_SetAtkDefFlags(&s->coll2, 0x13, 0x13);
    s->coll0.unkA &= ~4;
    s->coll1.unkA &= ~4;
    s->coll2.unkA &= ~4;
    if (prm != NULL) {
        s->unk1E0 = (UnkStruct_ObjParam *)prm;
        s->unk1E4 = ((UnkStruct_Prm *)s->unk1E0)->unk0;
        ((UnkStruct_Prm *)s->unk1E0)->unk0 = 0xff;
    }
    s->unk4A = 0x108 - data_ov000_023686e0[prm->unk2];
    *(void **)((u8 *)s + 0x130) = &s->unk1B8;
}
