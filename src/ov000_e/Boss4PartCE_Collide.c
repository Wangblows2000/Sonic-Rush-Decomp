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
} UnkStruct_022de964_C;

typedef struct {
    u8 pad0[0x134];
    UnkStruct_022de964_C coll0;
    UnkStruct_022de964_C coll1;
    UnkStruct_022de964_C coll2;
} UnkStruct_022de964_O;

typedef struct {
    u8 pad0[0x20C];
    UnkStruct_022de964_O *unk20C;
    u8 pad210[0x3A8 - 0x210];
    u8 unk3A8[4];
} UnkStruct_022de964_M;

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_022de964_M *gBoss4;
extern UnkStruct_02369ac0 gBossStatus;

void func_ov000_022db7ec(void *v, UnkStruct_022de964_O *o);
void ObjRect_SetBox2D(UnkStruct_022de964_C *c, s16 a, s16 b, s16 d, s16 e);
void StageObj_HandleCollider(void *w, void *a);
void func_ov000_022db894(UnkStruct_022de964_C *h, UnkStruct_022de964_C *out);
void Boss4_OnHit_Damage(void);

void Boss4PartCE_Collide(void) {
    UnkStruct_022de964_M *m = gBoss4;
    UnkStruct_022de964_O *o = m->unk20C;
    UnkStruct_022de964_C *c;

    func_ov000_022db7ec(m->unk3A8, o);
    if (gBossStatus.unk4 == 0x18)
        return;
    c = &o->coll0;
    ObjRect_SetBox2D(c, -0x28, 0, 0x50, 0x64);
    c->unk18 = o;
    c->unk1C = Boss4_OnHit_Damage;
    c->unkA |= 4;
    StageObj_HandleCollider(o, c);
    func_ov000_022db894(c, &o->coll2);
    o->coll2.unk4 = c->unk4;
}
