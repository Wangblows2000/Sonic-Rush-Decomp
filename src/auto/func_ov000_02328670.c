/* paramclone of func_ov000_02328170 (src/ov000/func_ov000_02328170.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct UnkStruct_02328670 {
    u8 pad[0x13e];
    u8 unk13e;
    u8 unk13f;
    u8 unk140;
} UnkStruct_02328670;

extern u8 data_ov000_02367dac;

UnkStruct_02328670 *func_ov000_02327ca4(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void func_ov000_02328670(s32 a, s32 b, s32 c, s32 d) {
    UnkStruct_02328670 *s = func_ov000_02327ca4(&data_ov000_02367dac, 3, a, b, c, d);
    s->unk13e = 0x14;
    s->unk13f = 3;
    s->unk140 = 0;
}
