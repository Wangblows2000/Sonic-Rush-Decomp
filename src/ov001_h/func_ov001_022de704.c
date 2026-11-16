#include <nitro/types.h>

typedef struct UnkStruct_022de704_w {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[0x5c - 8];
    u8 objs[2][0x138];
} UnkStruct_022de704_w;

void Model_Animate(void *a);
void Model_Draw(void *a);

void func_ov001_022de704(u8 *s, UnkStruct_022de704_w *w) {
    u8 *p;
    u8 i;
    for (i = 0; i < 2; i++) {
        p = w->objs[i];
        Model_Animate(p);
        Model_Draw(p);
    }
    p = s + 0xa84;
    *(u32 *)(p + 0x4c) = w->unk4;
    Model_Animate(p);
    Model_Draw(p);
}
