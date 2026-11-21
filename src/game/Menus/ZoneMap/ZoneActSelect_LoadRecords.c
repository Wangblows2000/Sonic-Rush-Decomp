#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 v[7][3];
} UnkStruct_022e0784;

extern s32 gGameState;
extern s32 data_022c4690[];
extern s32 data_022c46e8[];

#pragma thumb on
void ZoneActSelect_LoadRecords(UnkStruct_022e0784 *p) {
    s32 *tbl;
    s32 i;
    s32 j;

    if (gGameState == 0) {
        tbl = data_022c4690;
    } else {
        tbl = data_022c46e8;
    }
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 3; j++) {
            p->v[i][j] = tbl[i * 3 + j];
        }
    }
}
#pragma thumb reset
