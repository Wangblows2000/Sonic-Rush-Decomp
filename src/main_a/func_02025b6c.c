#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u8 count; } UnkStruct_02025b6c;
void func_02025c8c(UnkStruct_02025b6c *s, u32 idx, u32 v);

void func_02025b6c(UnkStruct_02025b6c *s, u32 v) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        func_02025c8c(s, i, v);
    }
}
