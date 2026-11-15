#include <nitro/types.h>

typedef struct UnkStruct_022c8dec_5 {
    s32 unk0;
} UnkStruct_022c8dec_5;

typedef struct UnkStruct_022c8dec_3 {
    u8 pad[0x304];
    s32 unk304;
    u8 pad1[4];
    u8 unk30C[0x418 - 0x30c];
    u8 unk418;
} UnkStruct_022c8dec_3;

typedef struct UnkStruct_022c8dec_2 {
    u8 pad[0x214];
    UnkStruct_022c8dec_3 *unk214;
} UnkStruct_022c8dec_2;

typedef struct UnkStruct_022c8dec UnkStruct_022c8dec;
struct UnkStruct_022c8dec {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x68 - 0x1c];
    void (*func)(UnkStruct_022c8dec *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x220 - 0x20e];
    s32 unk220;
    s32 unk224;
    u8 unk228[0x30c - 0x228];
    UnkStruct_022c8dec_5 *unk30C;
    u8 pad4[0x334 - 0x310];
    u8 unk334;
};

extern UnkStruct_022c8dec_2 *data_ov000_02369acc;

void func_ov000_022cc00c(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cc06c(void *p, s32 a, s32 b);

void func_ov000_022c8da0(UnkStruct_022c8dec *s);

void func_ov000_022c8dec(UnkStruct_022c8dec *s) {
    UnkStruct_022c8dec_3 *p = data_ov000_02369acc->unk214;
    func_ov000_022cc00c(s->unk228, s->unk220, 0x17, 0x1000, 0);
    func_ov000_022cc00c(p->unk30C, p->unk304, 0xa, 0x1000, 0);
    s->unk20C = 0;
    s->func = func_ov000_022c8da0;
}
