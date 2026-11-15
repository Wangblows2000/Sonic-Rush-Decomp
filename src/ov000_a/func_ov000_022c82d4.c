#include <nitro/types.h>

typedef struct UnkStruct_022c82d4_5 {
    s32 unk0;
} UnkStruct_022c82d4_5;

typedef struct UnkStruct_022c82d4_3 {
    u8 pad[0x304];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x418 - 0x30c];
    u8 unk418;
} UnkStruct_022c82d4_3;

typedef struct UnkStruct_022c82d4_2 {
    u8 pad[0x214];
    UnkStruct_022c82d4_3 *unk214;
} UnkStruct_022c82d4_2;

typedef struct UnkStruct_022c82d4 UnkStruct_022c82d4;
struct UnkStruct_022c82d4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x68 - 0x1c];
    void (*func)(UnkStruct_022c82d4 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x220 - 0x20e];
    s32 unk220;
    s32 unk224;
    u8 unk228[0x30c - 0x228];
    UnkStruct_022c82d4_5 *unk30C;
    u8 pad4[0x334 - 0x310];
    u8 unk334;
};

extern UnkStruct_022c82d4_2 *data_ov000_02369acc;

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc06c(void *p, s32 a, s32 b);

void func_ov000_022c82a0(UnkStruct_022c82d4 *s);

void func_ov000_022c82d4(UnkStruct_022c82d4 *s) {
    UnkStruct_022c82d4_3 *p = data_ov000_02369acc->unk214;
    func_ov000_022cc06c(&s->unk20C, 2, 6);
    if (s->unk30C->unk0 > 0xa000)
        return;
    s->unk334 |= 1;
    p->unk418 |= 1;
    s->func = func_ov000_022c82a0;
}
