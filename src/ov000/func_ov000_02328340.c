#include <nitro/types.h>

typedef struct UnkStruct_02328340 {
    u8 pad[0x13e];
    u8 unk13e;
    u8 unk13f;
    u8 unk140;
} UnkStruct_02328340;

extern u8 data_ov000_02367d0c;

UnkStruct_02328340 *func_ov000_02327ca4(void *p, s32 id, s32 a, s32 b, s32 c, s32 d);

void func_ov000_02328340(s32 a, s32 b, s32 c, s32 d) {
    UnkStruct_02328340 *s = func_ov000_02327ca4(&data_ov000_02367d0c, 3, a, b, c, d);
    s->unk13e = 0x13;
    s->unk13f = 3;
    s->unk140 = 0;
}
