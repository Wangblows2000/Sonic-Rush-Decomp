#include <nitro/types.h>

typedef struct UnkStruct_022c91d4_6 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022c91d4_6;

typedef struct UnkStruct_022c91d4_5 {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022c91d4_6 *unk8;
} UnkStruct_022c91d4_5;

typedef struct UnkStruct_022c91d4_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x304 - 0x14];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x418 - 0x30c];
    u8 unk418;
    u8 pad2[0x420 - 0x419];
    s32 unk420;
} UnkStruct_022c91d4_3;

typedef struct UnkStruct_022c91d4_2 {
    u8 pad[0x20c];
    u32 unk20C;
    u8 padA[4];
    UnkStruct_022c91d4_3 *unk214;
    u8 padB[0x228 - 0x218];
    s16 unk228;
} UnkStruct_022c91d4_2;

typedef struct UnkStruct_022c91d4 UnkStruct_022c91d4;
struct UnkStruct_022c91d4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x2e - 0x1c];
    u16 unk2E;
    u8 pad0b[0x68 - 0x30];
    void (*func)(UnkStruct_022c91d4 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x210 - 0x20e];
    u16 unk210;
    u8 pad2b[0x220 - 0x212];
    s32 unk220;
    s32 unk224;
    u8 unk228[0x30c - 0x228];
    UnkStruct_022c91d4_5 *unk30C;
    u8 pad4[0x334 - 0x310];
    u8 unk334;
    u8 pad5[0x33c - 0x335];
    s32 unk33C;
};

extern UnkStruct_022c91d4_2 *data_ov000_02369acc;

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc06c(void *p, s32 a, s32 b);

typedef struct UnkStruct_022c91d4_cam {
    u8 pad[0x68];
    s32 unk68;
    u8 pad1[0x78 - 0x6c];
    u16 unk78;
    u8 pad2[0x94 - 0x7a];
    s32 unk94;
    u8 pad4[0xb4 - 0x98];
    u16 unkB4;
} UnkStruct_022c91d4_cam;

extern UnkStruct_022c91d4_cam data_ov000_0236a444;

void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022c90e8(UnkStruct_022c91d4 *s);

void func_ov000_022c91d4(UnkStruct_022c91d4 *s) {
    UnkStruct_022c91d4_3 *p = data_ov000_02369acc->unk214;
    func_ov000_022cc00c(s->unk228, s->unk220, 0x15, 0x1000, 0);
    func_ov000_022cc00c(p->unk30C, p->unk304, 8, 0x1000, 0);
    data_ov000_0236a444.unk94 = -0x28000;
    data_ov000_0236a444.unkB4 = 0x80;
    data_ov000_0236a444.unk68 = 0xa0000;
    data_ov000_0236a444.unk78 = 0x55;
    func_02036754(0, 0, 0x81);
    s->unk20C = 0;
    s->func = func_ov000_022c90e8;
    s->func(s);
}
