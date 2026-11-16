#include <nitro/types.h>

typedef struct UnkStruct_02301234_3 {
    u8 pad[0x238];
    u32 unk238;
} UnkStruct_02301234_3;

typedef struct UnkStruct_02301234_2 {
    u8 pad[0x88];
    UnkStruct_02301234_3 *unk88;
} UnkStruct_02301234_2;

typedef struct UnkStruct_02301234 {
    u8 pad[0x10];
    UnkStruct_02301234_2 *unk10;
} UnkStruct_02301234;

void MapObject_Destructor(UnkStruct_02301234 *s);

void KuragenBomb_Destructor(UnkStruct_02301234 *s) {
    UnkStruct_02301234_3 *p = s->unk10->unk88;
    if (p != NULL)
        p->unk238 |= 0x40;
    MapObject_Destructor(s);
}
