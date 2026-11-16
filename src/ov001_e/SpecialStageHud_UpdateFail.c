#include <nitro/types.h>

typedef struct UnkStruct_02308d64 {
    u8 pad0[0xbbc];
    s32 unkBBC;
    u8 padBC0[0xbd0 - 0xbc0];
    s32 timer;
} UnkStruct_02308d64;

extern UnkStruct_02308d64 *gSpecialStageHud;
extern s16 gDisplayMain[];
extern s16 gDisplaySub[];

void SpecialStageMap_SetScrollSpeed(s32 v);
void *SpecialStage_GetPlayer(void);
void func_ov001_02312de0(void *p);
void SpecialStage_RequestExit(s32 v);

void SpecialStageHud_UpdateFail(void) {
    UnkStruct_02308d64 *g = gSpecialStageHud;
    s16 b;

    g->timer++;
    if (g->timer >= 0xb4) {
        b = g->timer - 0xb4;
        if (b > 0x10)
            b = 0x10;
        gDisplayMain[0x24] = b;
        gDisplaySub[0x24] = b;
    }
    if (g->timer <= 0x3c)
        SpecialStageMap_SetScrollSpeed(12288.0f * (1.0f - g->timer / 60.0f));
    switch (g->timer) {
    case 1:
        func_ov001_02312de0(SpecialStage_GetPlayer());
        g->unkBBC = 0;
        return;
    case 200:
        g->timer = -1;
        SpecialStage_RequestExit(0);
        return;
    }
}
