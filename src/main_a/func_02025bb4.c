#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u8 count; } UnkStruct_02025bb4;
void func_02025cd4(UnkStruct_02025bb4 *s, u32 idx, u32 v);

void func_02025bb4(UnkStruct_02025bb4 *s, u32 v) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        func_02025cd4(s, i, v);
    }
}
