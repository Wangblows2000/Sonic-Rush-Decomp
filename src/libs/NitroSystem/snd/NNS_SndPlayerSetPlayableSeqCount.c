#include <nitro/types.h>

typedef struct { u32 unk0; u8 unk4[0x20]; } UnkStruct_0201c138;
extern UnkStruct_0201c138 data_020838bc[];

void NNS_SndPlayerSetPlayableSeqCount(u32 idx, u32 v) {
    data_020838bc[idx].unk0 = (u16)v;
}
