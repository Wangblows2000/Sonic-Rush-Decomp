#include <nitro/types.h>

typedef struct UnkStruct_0235ab14 {
    u8 pad[0xa4];
    u8 unkA4[0xec - 0xa4];
    s32 unkEC;
    s32 unkF0;
    s32 unkF4;
} UnkStruct_0235ab14;

void Model_Draw(void *p);

void func_ov000_0235ab14(UnkStruct_0235ab14 *s, s32 *v) {
    s->unkEC = v[0] << 4;
    s->unkF0 = -v[1] << 4;
    s->unkF4 = 0;
    Model_Draw(s->unkA4);
}
