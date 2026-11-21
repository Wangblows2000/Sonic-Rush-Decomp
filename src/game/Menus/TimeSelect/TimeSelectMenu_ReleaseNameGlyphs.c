#include <nitro/types.h>

typedef struct {
    u8 pad0[0xFD8];
    void *unkFD8[2][7];
} UnkStruct_0235fce0;

void ObjVram_Free(void *a, void *b);

#pragma thumb on

void TimeSelectMenu_ReleaseNameGlyphs(UnkStruct_0235fce0 *w) {
    s32 j;
    s32 i;

    for (j = 0; j < 2; j++) {
        for (i = 0; i < 7; i++) {
            if (w->unkFD8[j][i] != NULL) {
                ObjVram_Free(NULL, w->unkFD8[j][i]);
                w->unkFD8[j][i] = NULL;
            }
        }
    }
}

#pragma thumb reset
