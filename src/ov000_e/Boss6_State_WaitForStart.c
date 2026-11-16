#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} UnkStruct_0236a524;

typedef struct {
    u32 unk0;
} UnkStruct_Flags;

typedef struct {
    u8 pad[0x14];
    s32 unk14;
} UnkStruct_02369b50;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0xF50 - 0x210];
    s32 unkF50;
} UnkStruct_Boss;

extern UnkStruct_Flags gBossStatus;
extern UnkStruct_Flags data_ov000_0236a444;
extern UnkStruct_0236a524 data_ov000_0236a524;
extern UnkStruct_02369b50 data_ov000_02369b50;

void Model_Init(void *p, s32 a);
void Model_SetResource(void *p, s32 a, s32 b);
void func_ov000_022e0700(void);
void func_ov000_022e5a80(void);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);

void Boss6_State_WaitForStart(UnkStruct_Boss *w) {
    UnkStruct_0236a524 *a;

    if (!(gBossStatus.unk0 & 1))
        return;
    a = (UnkStruct_0236a524 *)(u32)&data_ov000_0236a524;
    w->base.unk5C &= ~0x20;
    w->unk20C &= ~1;
    *(vu16 *)0x4000304 |= 0x8000;
    data_ov000_0236a444.unk0 |= 1;
    Model_Init(a, 0);
    Model_SetResource(a, data_ov000_02369b50.unk14, 0);
    a->unk18 = a->unk1C = a->unk20 = 0x1355b50;
    func_ov000_022e0700();
    func_ov000_022e5a80();
    w->unkF50 = 0xb4;
    func_020537b4(9);
    func_020533e0(0xf, 0xf0);
}
