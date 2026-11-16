#include <nitro/types.h>

typedef struct UnkStruct_023208a4 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_023208a4;

void DecGroupAF_State_ClearTouched(UnkStruct_023208a4 *s) {
    s->unkC = 0;
}
