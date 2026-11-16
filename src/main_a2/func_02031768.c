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

extern u8 data_0208fed4[];

void func_02021c4c(void *renderObj, void *anm);
void func_02025b38(void *alloc, void *anm);
void func_02039624(void *p);
void func_020393dc(void *p);

void func_02031768(UnkObj_02031768 *o) {
    u32 i;
    switch (o->type) {
    case 0:
        for (i = 0; i < 5; i++) {
            if (o->anm[i] != NULL) {
                func_02021c4c(o->renderObj, o->anm[i]);
                func_02025b38(data_0208fed4, o->anm[i]);
                o->anm[i] = NULL;
            }
            if (o->anm2[i] != NULL) {
                func_02021c4c(o->renderObj, o->anm2[i]);
                func_02025b38(data_0208fed4, o->anm2[i]);
                o->anm2[i] = NULL;
            }
        }
        break;
    case 1:
        break;
    case 2:
        if (o->unkC4 != NULL) {
            func_02039624(o->unkC4);
            o->unkC4 = NULL;
        }
        if (o->unkD0 != NULL) {
            func_020393dc(o->unkD0);
            o->unkD0 = NULL;
        }
        break;
    }
}
