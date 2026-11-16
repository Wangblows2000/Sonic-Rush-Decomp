#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x138 - 0x24];
} UnkStruct_022dfd80_Sub;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x54 - 0x1C];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x68 - 0x5C];
    void *unk68;
    u8 pad6C[0x124 - 0x6C];
    UnkStruct_022dfd80_Sub *unk124;
    u8 pad128[0x134 - 0x128];
    UnkStruct_022dfd80_Sub sub;
    u8 pad26C[0x2A0 - 0x26C];
} UnkStruct_022dfd80;

typedef struct {
    u8 pad0[0xD0C];
    void *unkD0C[4];
    void *unkD1C[4];
    void *unkD2C[4];
    void *unkD3C[4];
} UnkStruct_02369b4c;

extern UnkStruct_02369b4c *gBoss6;
extern void *data_ov000_023658ac[];

void func_ov000_022dfd74(void);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
s32 NNS_G3dGetTex(void *p);

UnkStruct_022dfd80 *func_ov000_022dfd80(s32 k, s32 x, s32 y, s32 z) {
    UnkStruct_022dfd80 *w;
    UnkStruct_022dfd80_Sub *sub;
    vu16 zero;

    w = Task_Create(StageObj_Main, func_ov000_022dfd74, 0, 0xa00, 4, sizeof(UnkStruct_022dfd80))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022dfd80));
    sub = &w->sub;
    w->unk124 = sub;
    Model_Init(sub, 0);
    Model_SetResource(sub, gBoss6->unkD0C[k], 0);
    if (gBoss6->unkD1C[k] != NULL)
        Model_SetAnimation(sub, 0, gBoss6->unkD1C[k], 0, 0);
    if (gBoss6->unkD2C[k] != NULL)
        Model_SetAnimation(sub, 1, gBoss6->unkD2C[k], 0, 0);
    if (gBoss6->unkD3C[k] != NULL)
        Model_SetAnimation(sub, 2, gBoss6->unkD3C[k], 0, NNS_G3dGetTex(gBoss6->unkD0C[k]));
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    w->unk10 = x;
    w->unk14 = y;
    w->unk18 = z;
    w->unk68 = data_ov000_023658ac[k];
    w->unk58 |= 0x300;
    w->unk54 |= 2;
    w->unkC = 0;
    w->unk8 = 0;
    return w;
}
