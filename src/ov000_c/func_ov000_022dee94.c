#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x10C - 0x54];
    u8 unk10C;
    u8 pad10D[0x138 - 0x10D];
} UnkStruct_022dee94_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022dee94_Sub unk20C;
    UnkStruct_022dee94_Sub unk344;
    u8 pad47C[0x4E0 - 0x47C];
    s32 unk4E0;
    s32 unk4E4;
    u8 unk4E8[0x4F8 - 0x4E8];
} UnkStruct_022dee94;

typedef struct {
    u8 pad0[0x210];
    UnkStruct_StageObj *unk210;
    u8 pad214[0xAB0 - 0x214];
    void *unkAB0[17];
} UnkStruct_02369b2c;

extern UnkStruct_02369b2c *data_ov000_02369b2c;

void func_ov000_022dbe38(void);
void func_ov000_022dbe6c(void);
void func_ov000_022dbfb4(void);
void func_ov000_022dbd5c(void);
void MI_CpuFill8(void *p, s32 v, u32 n);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_022db758(s32 x, void *a, s32 b, s32 c);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022dbd04(UnkStruct_022dee94 *w);

UnkStruct_022dee94 *func_ov000_022dee94(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_02369b2c *boss = data_ov000_02369b2c;
    UnkStruct_022dee94 *w;
    UnkStruct_022dee94_Sub *sub;

    w = Task_Create(StageObj_Main, func_ov000_022dbe38, 0, 0x505, 2, sizeof(UnkStruct_022dee94))->work;
    MI_CpuFill8(w, 0, sizeof(UnkStruct_022dee94));
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk70 = func_ov000_022dbe6c;
    w->base.unk68 = func_ov000_022dbfb4;
    w->base.unk78 = func_ov000_022dbd5c;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0xa300;
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, boss->unkAB0[12], 0);
    Model_SetAnimation(sub, 0, boss->unkAB0[13], 0, 0);
    sub->unk10C |= 4;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    sub->unk48 = 0;
    sub->unk4C = -0x200000;
    sub->unk50 = 0;
    w->base.unk10 = boss->unk210->unk10;
    w->base.unk14 = 0x20000;
    w->base.unk18 = 0;
    w->unk4E0 = w->base.unk10;
    w->unk4E4 = w->base.unk10;
    func_ov000_022db758(0, w->unk4E8, w->unk4E0, 0);
    sub = &w->unk344;
    Model_Init(sub, 0);
    Model_SetResource(sub, boss->unkAB0[14], 0);
    Model_SetAnimation(sub, 0, boss->unkAB0[15], 0, 0);
    Model_SetAnimation(sub, 1, boss->unkAB0[16], 0, 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    Sound_PlaySeqArc(0, 0, 0x8c);
    func_ov000_022dbd04(w);
    return w;
}
