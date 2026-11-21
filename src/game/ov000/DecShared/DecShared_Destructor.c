#include <nitro/types.h>

typedef struct UnkStruct_02322d14_2 {
    u8 pad[8];
    s32 unk8;
} UnkStruct_02322d14_2;

typedef struct UnkStruct_02322d14 {
    u8 pad[0x10];
    UnkStruct_02322d14_2 *unk10;
} UnkStruct_02322d14;

extern UnkStruct_02322d14 *gDecSharedObjects[];

void StageObj_Destructor(UnkStruct_02322d14 *s);

void DecShared_Destructor(UnkStruct_02322d14 *s) {
    s32 i = s->unk10->unk8;
    if (s == gDecSharedObjects[i])
        gDecSharedObjects[i] = NULL;
    StageObj_Destructor(s);
}
