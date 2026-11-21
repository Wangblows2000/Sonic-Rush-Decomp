#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 scale;
    u8 pad10[0x1C - 0x10];
    s32 ox;
    s32 oy;
    s32 oz;
} UnkStruct_023569d8;

void PlayerSelectMenu_DrawCharModel(UnkStruct_023569d8 *w);
void PlayerSelectMenu_DrawPartnerModel(UnkStruct_023569d8 *w);

#pragma thumb on

void PlayerSelectMenu_DrawModelPair(UnkStruct_023569d8 *w) {
    s32 x = w->x;
    s32 y = w->y;
    s32 z = w->z;
    s32 scale = w->scale;

    w->scale = (s32)(((s64)scale * 0x1000 + 0x800) >> 12);
    w->x = x + w->ox;
    w->y = y + w->oy;
    w->z = z + w->oz;
    PlayerSelectMenu_DrawCharModel(w);
    w->x = x - w->ox;
    w->y = y - w->oy;
    w->z = z - w->oz;
    PlayerSelectMenu_DrawPartnerModel(w);
    w->x = x;
    w->y = y;
    w->z = z;
    w->scale = scale;
}

#pragma thumb reset
