#include <nitro/types.h>

typedef struct UnkStruct_022daaf4_r {
    u8 pad[0x54];
    u32 unk54;
} UnkStruct_022daaf4_r;

typedef struct UnkStruct_022daaf4_b {
    u8 pad[0x214];
    void *unk214;
    UnkStruct_022daaf4_r *unk218;
} UnkStruct_022daaf4_b;

typedef struct UnkStruct_022daaf4_c {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x6c - 0x32];
    u16 unk6C;
    u8 pad2[0x80 - 0x6e];
    u16 unk80;
    u8 pad3[0x92 - 0x82];
    u16 unk92;
} UnkStruct_022daaf4_c;

extern UnkStruct_022daaf4_b *gBoss3;
extern UnkStruct_022daaf4_c data_ov000_0236a48c;

void func_ov000_022da6b4(void);
void Boss3_Action_Hurt(UnkStruct_022daaf4_b *b);
void Boss3PartCC_Action_Hurt(void *p);

void Boss3_Action_HurtAll(void) {
    UnkStruct_022daaf4_r *r = gBoss3->unk218;
    UnkStruct_022daaf4_c *c;
    if (r != NULL) {
        r->unk54 |= 8;
        gBoss3->unk218 = NULL;
    }
    c = (UnkStruct_022daaf4_c *)(u32)&data_ov000_0236a48c;
    func_ov000_022da6b4();
    c->unk30 = 0;
    c->unk6C = 0;
    c->unk80 = 0;
    c->unk92 = 0;
    Boss3_Action_Hurt(gBoss3);
    Boss3PartCC_Action_Hurt(gBoss3->unk214);
}
