#include <nitro/types.h>

typedef struct { u32 unk0; u8 unk4[0x20]; } UnkStruct_0201c120;
extern UnkStruct_0201c120 data_020838c0[];

void NNS_SndPlayerSetAllocatableChannel(u32 idx, u32 v) {
    data_020838c0[idx].unk0 = v;
}
