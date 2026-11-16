#include <nitro/types.h>

typedef struct UnkStruct_022de1e0 {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022de1e0;

void Model_Animate(void *a);
void Model_Draw(void *a);

void TransformDemo_Unk_ov001_022de1e0(UnkStruct_022de1e0 *s, u8 *w) {
    void *p;
    p = w + 0x18;
    Model_Animate(p);
    Model_Draw(p);
    p = w + 0x150;
    Model_Animate(p);
    Model_Draw(p);
    if (s->unk10 == 2) {
        p = w + 0x630;
        Model_Animate(p);
        Model_Draw(p);
    }
}
