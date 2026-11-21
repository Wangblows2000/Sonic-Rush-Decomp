#include <ov000_b/stageobj.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_0231f5d0;

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    UnkVec_0231f5d0 unk18;
    UnkVec_0231f5d0 unk24;
    UnkVec_0231f5d0 unk30;
    u8 pad3C[0x78 - 0x3C];
    s16 unk78;
    u8 pad7A[0xB4 - 0x7A];
    s16 unkB4;
    u8 padB6[0xC0 - 0xB6];
    s32 unkC0;
    s32 unkC4;
    s16 unkC8;
    u8 padCA[2];
    s32 unkCC;
    s32 unkD0;
    s16 unkD4;
    u16 unkD6;
    u16 unkD8;
    s16 unkDA;
    s32 unkDC;
    u8 padE0[0x218 - 0xE0];
} UnkStruct_0231f5d0;

extern UnkStruct_0231f5d0 data_ov000_0236a444;

void BossCamera_Main(void);
void BossCamera_Destructor(void);

static inline void SetVec(UnkVec_0231f5d0 *v, s32 x, s32 y, s32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void BossCamera_Create(s32 mode) {
    UnkStruct_0231f5d0 *c = &data_ov000_0236a444;
    vu16 zero;

    if (Task_Create(BossCamera_Main, BossCamera_Destructor, 0, 0x3f00, 3, 0) == NULL)
        return;
    zero = 0;
    MIi_CpuClear16(zero, c, sizeof(UnkStruct_0231f5d0));
    c->unkDC = mode;
    c->unk4 = 0x1555;
    c->unk8 = 0x1000;
    c->unkC = 0x800000;
    c->unk10 = 0x1555;
    c->unk14 = 0x1000;
    SetVec(&c->unk18, 0, 0x64000, 0xa000);
    SetVec(&c->unk24, 0, 0, 0);
    SetVec(&c->unk30, 0, 0x1000, 0);
    c->unkB4 = 0x200;
    c->unkC0 = c->unkC4 = 0xc8000;
    c->unkC8 = 0x200;
    c->unkCC = c->unkD0 = 0x3c000;
    c->unkD4 = 0x200;
    c->unkD6 = c->unkD8 = 0;
    c->unkDA = 0x200;
    c->unk78 = 0x200;
}
