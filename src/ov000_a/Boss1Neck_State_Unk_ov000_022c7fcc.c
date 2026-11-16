#include <nitro/types.h>

typedef struct UnkStruct_022c7fcc_5 {
    s32 unk0;
} UnkStruct_022c7fcc_5;

typedef struct UnkStruct_022c7fcc_3 {
    u8 pad[0x304];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x418 - 0x30c];
    u8 unk418;
} UnkStruct_022c7fcc_3;

typedef struct UnkStruct_022c7fcc_2 {
    u8 pad[0x214];
    UnkStruct_022c7fcc_3 *unk214;
} UnkStruct_022c7fcc_2;

typedef struct UnkStruct_022c7fcc UnkStruct_022c7fcc;
struct UnkStruct_022c7fcc {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x68 - 0x1c];
    void (*func)(UnkStruct_022c7fcc *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x220 - 0x20e];
    s32 unk220;
    s32 unk224;
    u8 unk228[0x30c - 0x228];
    UnkStruct_022c7fcc_5 *unk30C;
    u8 pad4[0x334 - 0x310];
    u8 unk334;
};

extern UnkStruct_022c7fcc_2 *gBoss1;

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc06c(void *p, s32 a, s32 b);

void Boss1Neck_State_Unk_ov000_022c7f80(UnkStruct_022c7fcc *s);

void Boss1Neck_State_Unk_ov000_022c7fcc(UnkStruct_022c7fcc *s) {
    UnkStruct_022c7fcc_3 *p = gBoss1->unk214;
    func_ov000_022cc00c(s->unk228, s->unk220, 0x12, 0x1000, 0);
    func_ov000_022cc00c(p->unk30C, p->unk304, 5, 0x1000, 0);
    s->unk20C = 0;
    s->func = Boss1Neck_State_Unk_ov000_022c7f80;
}
