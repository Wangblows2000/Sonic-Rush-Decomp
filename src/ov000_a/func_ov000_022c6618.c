#include <nitro/types.h>

typedef struct UnkStruct_022c6618 UnkStruct_022c6618;

typedef struct UnkStruct_022c6618_2 {
    u8 pad[0x210];
    UnkStruct_022c6618 *unk210;
    u8 pad1[0x228 - 0x214];
    s16 unk228;
} UnkStruct_022c6618_2;

struct UnkStruct_022c6618 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(UnkStruct_022c6618 *s);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2BC;
    u8 pad3[2];
    u16 unk2C0;
    u16 unk2C2;
    u8 pad4[0x2cc - 0x2c4];
    u16 unk2CC;
    s16 unk2CE;
};

extern UnkStruct_022c6618_2 *data_ov000_02369acc;

void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022c6548(UnkStruct_022c6618 *s);

void func_ov000_022c6618(UnkStruct_022c6618 *s) {
    u16 n;
    s16 v;
    n = (s->unk2BC > 4) ? 4 : s->unk2BC;
    v = ((n * 0x600) >> 2) + 0x600;
    if (data_ov000_02369acc->unk228 > 0)
        s->unk2E = -v;
    else
        s->unk2E = v;
    func_02036754(0, 0, 0x82);
    s->func = func_ov000_022c6548;
    s->func(s);
}
