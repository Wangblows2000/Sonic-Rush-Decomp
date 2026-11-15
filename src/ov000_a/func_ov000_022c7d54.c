#include <nitro/types.h>

typedef struct UnkStruct_022c7d54_6 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022c7d54_6;

typedef struct UnkStruct_022c7d54_5 {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022c7d54_6 *unk8;
} UnkStruct_022c7d54_5;

typedef struct UnkStruct_022c7d54 UnkStruct_022c7d54;
struct UnkStruct_022c7d54 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c7d54 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    u8 pad2[0x30c - 0x20e];
    UnkStruct_022c7d54_5 *unk30C;
    u8 pad3[0x334 - 0x310];
    u8 unk334;
};

typedef struct UnkStruct_022c7d54_2 {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad1[4];
    u8 *unk214;
} UnkStruct_022c7d54_2;

extern UnkStruct_022c7d54_2 *data_ov000_02369acc;

void func_02036754(s32 a, s32 b, s32 c);
void func_02000dfc(void *m);
void func_ov000_022c7d14(UnkStruct_022c7d54 *s);

void func_ov000_022c7d54(UnkStruct_022c7d54 *s) {
    UnkStruct_022c7d54_5 *q = s->unk30C;
    UnkStruct_022c7d54_2 *g = data_ov000_02369acc;
    if ((q->unk8->unk4 << 12) - q->unk0 < 0x14000 && s->unk20C == 0) {
        func_02036754(0, 0, 0x9e);
        s->unk20C = 1;
    }
    if (!(s->unk334 & 2))
        return;
    g->unk20C |= 1;
    func_02000dfc(g->unk214 + 0x330);
    s->func = func_ov000_022c7d14;
}
