#include <nitro/types.h>

typedef struct { u8 pad0[0x10]; s32 pal; u8 pad14[8]; u16 unk1C; u16 unk1E; } UnkRes_0204771c;
typedef struct { u8 pad0[0x14]; s32 unk14; u8 pad18[4]; u32 unk1C; u8 pad20[0x24]; } UnkBg_0204771c;
typedef struct {
    u8 pad0[0x348];
    UnkRes_0204771c *res348;
    UnkRes_0204771c *res34C;
} UnkWork_0204771c;
typedef struct { s32 unk0; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;

void Background_Init(UnkBg_0204771c *bg, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(UnkBg_0204771c *bg);
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);

#pragma thumb on
void ClearDemo_InitBgBoss(UnkWork_0204771c *w) {
    s32 pal;
    UnkRes_0204771c *res;
    UnkBg_0204771c bg;

    if (gGameState.unk0 != 0)
        pal = 1;
    else
        pal = 0;
    Background_Init(&bg, w->res34C, 0x31, 0, 1, 0x20, 0x18);
    Background_Draw(&bg);
    res = w->res34C;
    PaletteTransfer_QueueCopy((u8 *)res + res->pal + 4 + pal * 32, 0x10, bg.unk14, (res->unk1C << 1) + 0x05000000);
    *(vu16 *)0x05000000 = ((vu16 *)0x05000000)[w->res34C->unk1C];
    Background_Init(&bg, w->res348, 0x31, 0, 2, 0x20, 0x18);
    bg.unk1C = 0x06006800;
    Background_Draw(&bg);
    res = w->res348;
    PaletteTransfer_QueueCopy((u8 *)res + res->pal + 4 + pal * 32, 0x10, bg.unk14, (res->unk1C << 1) + 0x05000000);
    Background_Init(&bg, w->res348, 0x23, 0, 3, 0x20, 0x18);
    Background_Draw(&bg);
}
#pragma thumb reset
