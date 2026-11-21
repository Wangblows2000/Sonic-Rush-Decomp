#include <nitro/types.h>

typedef struct { u8 data[0xf4]; } UnkStruct_0230f448_a;
typedef struct { u8 data[0x138]; } UnkStruct_0230f448_b;
extern u8 *gSpecialStageGimmickModels;
void Model_Animate(void *p);
void SpecialStageObjList_Update(void *p);

void SpecialStageGimmick_Update(void **p) {
    u8 *g = gSpecialStageGimmickModels;
    s32 i;
    for (i = 0; i < 9; i++) {
        Model_Animate(&((UnkStruct_0230f448_a *)(g + 4))[i]);
    }
    for (i = 0; i < 5; i++) {
        Model_Animate(&((UnkStruct_0230f448_b *)(g + 0x8ac))[i]);
    }
    SpecialStageObjList_Update(*p);
}
