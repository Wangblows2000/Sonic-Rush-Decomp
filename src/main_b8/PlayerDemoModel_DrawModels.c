#include <nitro/types.h>

typedef struct {
    u8 pad0[0x138];
} UnkModel_020563e4;

typedef struct {
    u8 pad0[0x20];
    s8 count;
    s8 cur;
    u8 pad22[0x1a];
    UnkModel_020563e4 models[1];
} UnkWork_020563e4;

void Model_Animate(UnkModel_020563e4 *m);
void Model_Draw(UnkModel_020563e4 *m);

#pragma thumb on
void PlayerDemoModel_DrawModels(UnkWork_020563e4 *w) {
    u8 i;

    for (i = 0; i < w->count; i++) {
        UnkModel_020563e4 *m = &w->models[i];
        Model_Animate(m);
        Model_Draw(m);
    }
}
#pragma thumb reset
