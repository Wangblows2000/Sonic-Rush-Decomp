#include <nitro/types.h>

typedef struct { fx32 m[4][3]; fx32 unk30; fx32 unk34; } UnkStruct_02029480;

void func_02029480(UnkStruct_02029480 *s) {
    s->m[0][0] = 0x1000;
    s->m[0][1] = 0;
    s->m[1][1] = 0;
    s->m[1][2] = 0x1000;
    s->unk30 = 0;
    s->unk34 = 0;
}
