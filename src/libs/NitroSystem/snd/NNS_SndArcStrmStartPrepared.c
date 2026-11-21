#include <nitro/types.h>

typedef struct { u8 unk0[0x10C]; u32 unk10C; } UnkStruct_02020260;

void NNS_SndArcStrmStartPrepared(UnkStruct_02020260 **pp) {
    UnkStruct_02020260 *p = *pp;
    if (p != NULL) {
        p->unk10C |= 4;
    }
}
