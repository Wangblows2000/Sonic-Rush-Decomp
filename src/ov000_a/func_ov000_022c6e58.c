#include <nitro/types.h>

typedef struct UnkStruct_022c6e58 UnkStruct_022c6e58;

typedef struct UnkStruct_022c6e58_2 {
    u8 pad[0x210];
    UnkStruct_022c6e58 *unk210;
    u8 pad1[0x228 - 0x214];
    s16 unk228;
} UnkStruct_022c6e58_2;

struct UnkStruct_022c6e58 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(UnkStruct_022c6e58 *s);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2BC;
    u16 unk2BE;
    u16 unk2C0;
    u16 unk2C2;
    u8 pad4[0x2cc - 0x2c4];
    u16 unk2CC;
    s16 unk2CE;
};

extern UnkStruct_022c6e58_2 *data_ov000_02369acc;

extern u32 gRandSeed;
extern s16 data_0206e470[];

u8 *EffPebble_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022c6e58(UnkStruct_022c6e58 *s) {
    s16 v;
    s32 a;
    s16 b;
    u8 *obj;
    if (s->unk2BE++ & 3)
        return;
    v = s->unk2CE >> 4;
    if (v > 0)
        v += (Rand() & 0xff) - 0x80;
    else if (v < 0)
        v -= (Rand() & 0xff) - 0x80;
    a = s->unk2CE;
    if (a < 0)
        a = -a;
    b = -(a >> 3);
    b = b - (s16)(Rand() & 0xff);
    obj = EffPebble_Create(s->unk10, 0x20000, 0x1000 - (Rand() & 0x1fff), v, b, (s16)(0x100 - (Rand() & 0x1ff)), 0x3c);
    *(s32 *)(obj + 0x14c) += (Rand() & 0x3fff) - 0x1000;
    *(s32 *)(obj + 0x150) += (Rand() & 0x3fff) - 0x1000;
    *(s32 *)(obj + 0x154) += (Rand() & 0x3fff) - 0x1000;
    switch ((s->unk2BE >> 2) & 3) {
    case 0:
        MTX_RotX33_(obj + 0x158, data_0206e470[((s->unk2CC & 0xffff) >> 4) * 2], data_0206e470[((s->unk2CC & 0xffff) >> 4) * 2 + 1]);
        break;
    case 1:
        MTX_RotY33_(obj + 0x158, data_0206e470[((s->unk2CC & 0xffff) >> 4) * 2], data_0206e470[((s->unk2CC & 0xffff) >> 4) * 2 + 1]);
        break;
    case 2:
    default:
        MTX_RotZ33_(obj + 0x158, data_0206e470[((s->unk2CC & 0xffff) >> 4) * 2], data_0206e470[((s->unk2CC & 0xffff) >> 4) * 2 + 1]);
        break;
    }
}
