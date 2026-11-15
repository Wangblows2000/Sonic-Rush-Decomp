#include <nitro/types.h>

typedef struct UnkStruct_0201c7cc UnkStruct_0201c7cc;
typedef void (*UnkCb_0201c7cc)(u8 *a, u8 *b, u32 size, u32 arg, u32 arg2);
struct UnkStruct_0201c7cc {
    u8 unk0[8];
    u32 unk8;
    u8 *unkC;
    u8 *unk10;
    u8 unk14[4];
    u32 unk18;
    s32 unk1C;
    u8 unk20[0x10];
    s32 unk30;
    UnkCb_0201c7cc unk34;
    u32 unk38;
};

typedef struct { UnkStruct_0201c7cc *obj; u32 size; u32 offset; u8 *a; u8 *b; } UnkEntry_0201c7cc;

extern u32 data_02083df8;
extern s32 data_02083df4;
extern UnkEntry_0201c7cc data_02083e64[];
extern u8 data_02083dfc[];

BOOL func_020064a0(void *queue, void *msg, s32 flags);
void DC_InvalidateRange(void *p, u32 size);

void func_0201c7cc(UnkStruct_0201c7cc *s) {
    u8 *a;
    u8 *b;
    u32 size;
    u32 off;
    size = s->unk18;
    off = size * s->unk1C;
    a = s->unkC + off;
    b = s->unk10 + off;
    if (data_02083df8) {
        UnkEntry_0201c7cc *e = &data_02083e64[data_02083df4];
        e->obj = s;
        e->size = size;
        e->offset = off;
        e->a = a;
        e->b = b;
        func_020064a0(data_02083dfc, e, 0);
        if (++data_02083df4 >= 8) data_02083df4 = 0;
    } else {
        DC_InvalidateRange(a, size);
        DC_InvalidateRange(b, size);
        s->unk34(a, b, size, s->unk8, s->unk38);
    }
    s->unk1C++;
    if (s->unk1C >= s->unk30) s->unk1C = 0;
}
