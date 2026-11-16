#include <nitro/types.h>

typedef struct { u8 data[0xf4]; } UnkStruct_0230f448_a;
typedef struct { u8 data[0x138]; } UnkStruct_0230f448_b;
extern u8 *data_ov001_0231f658;
void Model_Animate(void *p);
void func_ov001_02310af4(void *p);

void func_ov001_0230f448(void **p) {
    u8 *g = data_ov001_0231f658;
    s32 i;
    for (i = 0; i < 9; i++) {
        Model_Animate(&((UnkStruct_0230f448_a *)(g + 4))[i]);
    }
    for (i = 0; i < 5; i++) {
        Model_Animate(&((UnkStruct_0230f448_b *)(g + 0x8ac))[i]);
    }
    func_ov001_02310af4(*p);
}
