#include <nitro/types.h>

typedef struct { u8 *unk0; s32 unk4; } UnkEntry_0201c224;
extern UnkEntry_0201c224 data_02083d74[];
extern u8 *data_02083d34[];

typedef void (*UnkCb_0201c224)(u32 v, s32 count, u8 **bufs, u32 size, s32 format, s32 arg);

typedef struct {
    u8 unk0[0x20];
    s32 unk20;
    u32 unk24;
    u32 unk28;
    s32 unk2C;
    UnkCb_0201c224 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 unk44[4];
    s32 count;
    u8 ids[1];
} UnkStruct_0201c224;

u32 _u32_div_f(u32 a, u32 b);

void func_0201c224(UnkStruct_0201c224 *s, u32 v) {
    u32 size = _u32_div_f(s->unk28, s->unk2C);
    u32 off = size * s->unk38;
    s32 i;
    for (i = 0; i < s->count; i++) {
        data_02083d34[i] = off + data_02083d74[s->ids[i]].unk0;
    }
    s->unk30(v, s->count, data_02083d34, size, s->unk20, s->unk34);
    s->unk38++;
    if (s->unk38 >= s->unk2C) s->unk38 = 0;
}
