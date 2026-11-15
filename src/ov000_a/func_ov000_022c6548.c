#include <nitro/types.h>

typedef struct UnkStruct_022c6548 UnkStruct_022c6548;

typedef struct UnkStruct_022c6548_2 {
    u8 pad[0x210];
    UnkStruct_022c6548 *unk210;
    u8 pad1[0x228 - 0x214];
    s16 unk228;
} UnkStruct_022c6548_2;

struct UnkStruct_022c6548 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(UnkStruct_022c6548 *s);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2BC;
    u8 pad3[2];
    u16 unk2C0;
    u16 unk2C2;
    u8 pad4[0x2cc - 0x2c4];
    u16 unk2CC;
    s16 unk2CE;
};

extern UnkStruct_022c6548_2 *data_ov000_02369acc;

void func_ov000_022c6e58(UnkStruct_022c6548 *s);
void func_ov000_022c7158(UnkStruct_022c6548 *s);
void func_ov000_022c64a8(UnkStruct_022c6548 *s);
void func_ov000_022c61ec(UnkStruct_022c6548 *s);

void func_ov000_022c6548(UnkStruct_022c6548 *s) {
    BOOL b = FALSE;
    s32 v;
    if (s->unk2CE > 0)
        s->unk2CE -= 0xa0;
    else if (s->unk2CE < 0)
        s->unk2CE += 0xa0;
    s->unk2CC += s->unk2CE;
    func_ov000_022c6e58(s);
    v = data_ov000_02369acc->unk228;
    if (v > 0) {
        if (s->unk10 <= 0x9000)
            b = TRUE;
    } else {
        if (s->unk10 >= 0x13200)
            b = TRUE;
    }
    if (b) {
        if (v < 0)
            v = -v;
        if (v == 1)
            s->func = func_ov000_022c64a8;
        else
            s->func = func_ov000_022c61ec;
    }
    func_ov000_022c7158(s);
}
