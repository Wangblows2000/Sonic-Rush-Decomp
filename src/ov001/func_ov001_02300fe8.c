#include <nitro/types.h>

typedef struct { u8 pad[8]; void *a[5]; void *b[10]; } UnkStruct_0231f530;
extern UnkStruct_0231f530 *gExtraBossPlayerManager;
void func_ov001_02301118(void *p);

void func_ov001_02300fe8(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (gExtraBossPlayerManager->b[i] != NULL) {
            func_ov001_02301118(gExtraBossPlayerManager->b[i]);
        }
    }
    for (i = 0; i < 5; i++) {
        if (gExtraBossPlayerManager->a[i] != NULL) {
            func_ov001_02301118(gExtraBossPlayerManager->a[i]);
        }
    }
}
