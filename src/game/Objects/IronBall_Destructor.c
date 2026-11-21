#include <nitro/types.h>

typedef struct UnkStruct_0230d270_2 {
    u8 pad[0x1dc];
    void *unk1DC;
} UnkStruct_0230d270_2;

typedef struct UnkStruct_0230d270 {
    u8 pad[0x10];
    UnkStruct_0230d270_2 *unk10;
} UnkStruct_0230d270;

void NNS_SndPlayerStopSeq(void *p, s32 a);
void MapObject_Destructor(UnkStruct_0230d270 *s);

void IronBall_Destructor(UnkStruct_0230d270 *s) {
    if (s->unk10->unk1DC != NULL)
        NNS_SndPlayerStopSeq(s->unk10->unk1DC, 0);
    MapObject_Destructor(s);
}
