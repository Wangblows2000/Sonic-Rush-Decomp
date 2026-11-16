#include <nitro/types.h>

typedef struct { u8 pad0[0x10]; s32 pal; } UnkRes_0205e09c;
typedef struct { u8 pad0[0x14]; s32 unk14; u8 pad18[4]; u32 unk1c; u8 pad20[0x20]; } UnkBg_0205e09c;
typedef struct { s32 m[4]; } UnkMtx22_0205e09c;

typedef struct {
    u8 pad0[0x47c];
    s32 idx;
    u8 pad480[0x18];
    UnkRes_0205e09c *res498;
    UnkRes_0205e09c *res49c;
} UnkWork_0205e09c;

extern u16 data_02076b48[];
extern u8 data_02087d3c[];

void Background_Init(UnkBg_0205e09c *bg, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(UnkBg_0205e09c *bg);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);
void MTX_Identity22_(UnkMtx22_0205e09c *m);
void G2x_SetBGyAffine_(void *p, UnkMtx22_0205e09c *m, s32 x, s32 y, s32 a, s32 b);

void func_0205e09c(UnkWork_0205e09c *w) {
    u32 ofs;
    UnkBg_0205e09c bg;
    UnkMtx22_0205e09c m;

    ofs = data_02076b48[w->idx];
    Background_Init(&bg, w->res49c, 1, 0, 2, 0x20, 0x20);
    bg.unk1c = 0x06006800;
    Background_Draw(&bg);
    PaletteTransfer_QueueCopy((void *)(ofs + ((u32)w->res498 + w->res498->pal + 4)), 0x10, bg.unk14, 0x4400);
    Background_Init(&bg, w->res498, 1, 1, 3, 0x20, 0x18);
    bg.unk1c = 0x06206b80;
    Background_Draw(&bg);
    PaletteTransfer_QueueCopy((void *)(ofs + ((u32)w->res498 + w->res498->pal + 4)), 0x10, bg.unk14, 0x6400);
    Background_Init(&bg, w->res49c, 1, 1, 2, 0x20, 0x20);
    bg.unk1c = 0x06206800;
    Background_Draw(&bg);
    PaletteTransfer_QueueCopy((void *)(ofs + ((u32)w->res498 + w->res498->pal + 4)), 0x10, bg.unk14, 0x4400);
    MTX_Identity22_(&m);
    G2x_SetBGyAffine_(data_02087d3c, &m, 0x80, 0x60, 0, 0);
}
