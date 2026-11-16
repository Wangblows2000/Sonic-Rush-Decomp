#include <nitro/types.h>

typedef struct UnkStruct_022dc504 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022dc504;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022dc504 *s);

void Boss4PartD1_Destructor(UnkStruct_022dc504 *s) {
    Model_Release(s->unk10 + 0x20c);
    MapObject_Destructor(s);
}
