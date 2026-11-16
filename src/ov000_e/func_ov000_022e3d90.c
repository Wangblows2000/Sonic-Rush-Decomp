#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD54 - 0xD0C];
    s32 unkD54;
    s32 unkD58;
    u8 padD5C[0xECA - 0xD5C];
    u16 unkECA;
    u8 padECC[0xF68 - 0xECC];
    s16 unkF68;
    u8 padF6A[2];
    s16 unkF6C;
} UnkStruct_Boss;

void func_ov000_022e3954(UnkStruct_Boss *w);
void func_ov000_022e37f0(UnkStruct_Boss *w);
void func_ov000_022e1f10(UnkStruct_Boss *w);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_022e01a4(UnkStruct_Boss *w);
void func_ov000_022e3cec(void);

void func_ov000_022e3d90(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    func_ov000_022e3954(w);
    func_ov000_022e37f0(w);
    func_ov000_022e1f10(w);
    if (w->unkF68 != w->unkF6C)
        return;
    w->base.unk68 = func_ov000_022e3cec;
    Model_SetAnimation(m, 0, w->unkD08, 0xb, 0);
    m->unk114 = 0x1000;
    w->unkECA &= ~4;
    w->unkECA |= 0x1000;
    w->unk20C &= ~8;
    func_ov000_022e01a4(w);
    w->unkD54 = -0x17ff;
    w->unkD58 = 0x17ff;
}
