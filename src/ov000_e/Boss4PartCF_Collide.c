#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[2];
    u16 unkA;
    u8 padC[0x18 - 0xC];
    void *unk18;
    void *unk1C;
    u32 unk20;
    u8 pad24[0x2C - 0x24];
} UnkStruct_022de860_C;

typedef struct {
    u8 pad0[0x134];
    UnkStruct_022de860_C coll0;
    UnkStruct_022de860_C coll1;
    UnkStruct_022de860_C coll2;
} UnkStruct_022de860_O;

typedef struct {
    u8 pad0[0x210];
    UnkStruct_022de860_O *unk210;
    u8 pad214[0x4E0 - 0x214];
    u8 unk4E0[0xBD0 - 0x4E0];
    s32 unkBD0;
    u8 padBD4[0xBF0 - 0xBD4];
    s32 unkBF0;
} UnkStruct_022de860_M;

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_022de860_M *gBoss4;
extern UnkStruct_02369ac0 gBossStatus;

void Boss4_ArenaToStagePos(void *v, UnkStruct_022de860_O *o);
void ObjRect_SetBox2D(UnkStruct_022de860_C *c, s16 a, s16 b, s16 d, s16 e);
void StageObj_HandleCollider(void *w, void *a);
void Boss4_HandleColliderWrapped(UnkStruct_022de860_C *h, UnkStruct_022de860_C *out);
void Boss4_OnHit_Damage(void);

void Boss4PartCF_Collide(void) {
    UnkStruct_022de860_M *m = gBoss4;
    UnkStruct_022de860_O *o = m->unk210;
    UnkStruct_022de860_C *c;

    Boss4_ArenaToStagePos(m->unk4E0, o);
    if (m->unkBD0 != 0 || gBossStatus.unk4 == 0x18) {
        c = &o->coll1;
        ObjRect_SetBox2D(c, -0x4b, -0x28, 0x4b, 0x28);
        c->unk1C = NULL;
        if (m->unkBF0 != 0)
            c->unk20 |= 0x20;
        else
            c->unk20 &= ~0x20;
    } else {
        c = &o->coll0;
        ObjRect_SetBox2D(c, -0x32, -0x23, 0x32, 0x23);
        c->unk1C = Boss4_OnHit_Damage;
    }
    c->unk18 = o;
    c->unkA |= 4;
    StageObj_HandleCollider(o, c);
    c->unk4 += 0x64;
    Boss4_HandleColliderWrapped(c, &o->coll2);
    o->coll2.unk4 = c->unk4;
}
