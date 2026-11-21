#include <nitro/types.h>

typedef struct { u8 pad[8]; void *a[5]; void *b[10]; } UnkStruct_0231f530;
extern UnkStruct_0231f530 *gExtraBossPlayerManager;
void ExtraBossObj_Destroy(void *p);

void ExtraBossObj_DestroyKind2And3(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (gExtraBossPlayerManager->b[i] != NULL) {
            ExtraBossObj_Destroy(gExtraBossPlayerManager->b[i]);
        }
    }
    for (i = 0; i < 5; i++) {
        if (gExtraBossPlayerManager->a[i] != NULL) {
            ExtraBossObj_Destroy(gExtraBossPlayerManager->a[i]);
        }
    }
}
