#include <nitro/types.h>

typedef struct { u8 data[0xf4]; } UnkStruct_02306a20_a;
typedef struct { u8 data[0x138]; } UnkStruct_02306a20_b;
typedef struct {
    UnkStruct_02306a20_a a[0x20];
    void *aref[0x20];
    UnkStruct_02306a20_b b[0x20];
    void *bref[0x20];
} UnkStruct_0231f5b8;
extern UnkStruct_0231f5b8 *data_ov001_0231f5b8;

void SpecialStage_Unk_ov001_02306a20(u8 *o) {
    UnkStruct_0231f5b8 *g = data_ov001_0231f5b8;
    s32 i;
    if (*(void **)(o + 0xb0) != NULL) {
        for (i = 0; i < 0x20; i++) {
            if (&g->a[i] == *(void **)(o + 0xb0)) {
                g->aref[i] = NULL;
                break;
            }
        }
    }
    if (*(void **)(o + 0xac) != NULL) {
        for (i = 0; i < 0x20; i++) {
            if (&g->b[i] == *(void **)(o + 0xac)) {
                g->bref[i] = NULL;
                return;
            }
        }
    }
}
