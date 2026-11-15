#include <nitro/types.h>

typedef struct { u8 unk0[0x148]; u32 unk148; } UnkStruct_02020198;

void func_02020198(UnkStruct_02020198 **pp) {
    UnkStruct_02020198 *p = *pp;
    if (p != NULL) {
        p->unk148 = 0;
        *pp = NULL;
    }
}
