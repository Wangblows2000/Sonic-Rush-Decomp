#include <nitro/types.h>

typedef struct {
    u8 pad0[0xDDC];
    void *unkDDC[2][7];
} UnkStruct_0235d158;

void ObjVram_Free(void *a, void *b);

#pragma thumb on

void func_ov000_0235d158(UnkStruct_0235d158 *w) {
    s32 j;
    s32 i;

    for (j = 0; j < 2; j++) {
        for (i = 0; i < 7; i++) {
            if (w->unkDDC[j][i] != NULL) {
                ObjVram_Free(NULL, w->unkDDC[j][i]);
                w->unkDDC[j][i] = NULL;
            }
        }
    }
}

#pragma thumb reset
