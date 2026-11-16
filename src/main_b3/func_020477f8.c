#include <nitro/types.h>

typedef struct { u8 pad0[0x10]; s32 pal; u8 pad14[8]; u16 unk1C; u16 unk1E; } UnkRes_020477f8;
typedef struct { u8 pad0[0x14]; s32 unk14; u8 pad18[4]; u32 unk1C; u8 pad20[0x20]; } UnkBg_020477f8;
typedef struct {
    u8 pad0[0x348];
    UnkRes_020477f8 *res348;
    UnkRes_020477f8 *res34C;
} UnkWork_020477f8;
typedef struct { s32 unk0; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;

void Background_Init(UnkBg_020477f8 *bg, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(UnkBg_020477f8 *bg);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);

#pragma thumb on
void func_020477f8(UnkWork_020477f8 *w) {
    s32 pal;
    UnkRes_020477f8 *res;
    UnkBg_020477f8 bg;

    if (data_022c4560.unk0 != 0)
        pal = 1;
    else
        pal = 0;
    Background_Init(&bg, w->res34C, 0x31, 0, 1, 0x20, 0x18);
    Background_Draw(&bg);
    res = w->res34C;
    PaletteTransfer_QueueCopy((u8 *)res + res->pal + 4 + pal * 32, 0x10, bg.unk14, (res->unk1E << 9) + 0x6000 + (res->unk1C << 1));
    Background_Init(&bg, w->res348, 0x31, 0, 2, 0x20, 0x18);
    bg.unk1C = 0x06006800;
    Background_Draw(&bg);
    res = w->res348;
    PaletteTransfer_QueueCopy((u8 *)res + res->pal + 4 + pal * 32, 0x10, bg.unk14, (res->unk1E << 9) + 0x4000 + (res->unk1C << 1));
    Background_Init(&bg, w->res348, 0x23, 0, 3, 0x20, 0x18);
    Background_Draw(&bg);
    res = w->res348;
    PaletteTransfer_QueueCopy((u8 *)res + res->pal + 4 + pal * 32, 0x10, bg.unk14, (res->unk1E << 9) + 0x6000 + (res->unk1C << 1));
}
#pragma thumb reset
