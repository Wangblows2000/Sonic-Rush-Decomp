#include <nitro/types.h>

typedef struct {
    u32 type;
    u8 unk4[0x8C];
    u8 renderObj[0x34];
    void *unkC4;
    u8 unkC8[8];
    void *unkD0;
    u8 unkD4[0x10];
    void *anm[5];
    void *anm2[5];
} UnkObj_02031768;

extern u8 gSystemHeapAllocator[];

void func_02021c4c(void *renderObj, void *anm);
void func_02025b38(void *alloc, void *anm);
void VramTex_Free(void *p);
void VramPltt_Free(void *p);

void Model_Release(UnkObj_02031768 *o) {
    u32 i;
    switch (o->type) {
    case 0:
        for (i = 0; i < 5; i++) {
            if (o->anm[i] != NULL) {
                func_02021c4c(o->renderObj, o->anm[i]);
                func_02025b38(gSystemHeapAllocator, o->anm[i]);
                o->anm[i] = NULL;
            }
            if (o->anm2[i] != NULL) {
                func_02021c4c(o->renderObj, o->anm2[i]);
                func_02025b38(gSystemHeapAllocator, o->anm2[i]);
                o->anm2[i] = NULL;
            }
        }
        break;
    case 1:
        break;
    case 2:
        if (o->unkC4 != NULL) {
            VramTex_Free(o->unkC4);
            o->unkC4 = NULL;
        }
        if (o->unkD0 != NULL) {
            VramPltt_Free(o->unkD0);
            o->unkD0 = NULL;
        }
        break;
    }
}
