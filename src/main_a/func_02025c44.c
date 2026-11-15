#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u8 count; } UnkStruct_02025c44;
void func_02025d64(UnkStruct_02025c44 *s, u32 idx, u32 v);

void func_02025c44(UnkStruct_02025c44 *s, u32 v) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        func_02025d64(s, i, v);
    }
}
