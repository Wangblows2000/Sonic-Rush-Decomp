#include <nitro/types.h>

typedef struct UnkStruct_022c72f8_2 {
    u8 pad[4];
    u8 *unk4;
} UnkStruct_022c72f8_2;

typedef struct UnkStruct_022c72f8 {
    u8 pad[4];
    UnkStruct_022c72f8_2 *unk4;
    u32 unk8;
    u8 pad1[0x24 - 0xc];
    s32 unk24;
    u8 pad2[0x92 - 0x28];
    u8 unk92;
    u8 pad3[0xae - 0x93];
    u8 unkAE;
} UnkStruct_022c72f8;

extern u8 data_ov000_02362bb4;

s32 func_02025edc(void *p, void *q);
void func_02025268(s32 a, void *b, s32 c);

void func_ov000_022c72f8(UnkStruct_022c72f8 *s) {
    s32 buf;
    s32 v = func_02025edc(s->unk4->unk4 + 0x40, &data_ov000_02362bb4);
    if (v != ((s->unk8 & 0x10) ? s->unkAE : -1))
        return;
    buf = 0x1e;
    func_02025268(0x13, &buf, 1);
    s->unk24 = 0;
    s->unk92 = 0;
}
