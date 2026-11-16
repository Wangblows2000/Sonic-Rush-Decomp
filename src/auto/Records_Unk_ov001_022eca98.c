/* paramclone of ResultsScreen_ReleaseNameGlyphs (src/ov000_d/ResultsScreen_ReleaseNameGlyphs.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    u8 pad0[0x11a0];
    void *unkDDC[11][7];
} UnkStruct_022eca98;

void ObjVram_Free(void *a, void *b);

#pragma thumb on

void Records_Unk_ov001_022eca98(UnkStruct_022eca98 *w) {
    s32 j;
    s32 i;

    for (j = 0; j < 11; j++) {
        for (i = 0; i < 7; i++) {
            if (w->unkDDC[j][i] != NULL) {
                ObjVram_Free(NULL, w->unkDDC[j][i]);
                w->unkDDC[j][i] = NULL;
            }
        }
    }
}

#pragma thumb reset
