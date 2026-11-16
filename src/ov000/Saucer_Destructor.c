#include <nitro/types.h>

typedef struct UnkStruct_02315a64_2 {
    u8 pad[0x1dc];
    void *unk1DC;
} UnkStruct_02315a64_2;

typedef struct UnkStruct_02315a64 {
    u8 pad[0x10];
    UnkStruct_02315a64_2 *unk10;
} UnkStruct_02315a64;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void MapObject_Destructor(UnkStruct_02315a64 *s);

void Saucer_Destructor(UnkStruct_02315a64 *s) {
    NNS_SndPlayerStopSeq(s->unk10->unk1DC, 0);
    MapObject_Destructor(s);
}
