#include <nitro/types.h>

typedef struct { u8 unk0[0x148]; u32 unk148; } UnkStruct_02020198;

void NNS_SndStrmHandleRelease(UnkStruct_02020198 **pp) {
    UnkStruct_02020198 *p = *pp;
    if (p != NULL) {
        p->unk148 = 0;
        *pp = NULL;
    }
}
