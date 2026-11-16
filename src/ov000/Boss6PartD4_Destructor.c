#include <nitro/types.h>

typedef struct UnkStruct_022e63ec {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022e63ec;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022e63ec *s);

void Boss6PartD4_Destructor(UnkStruct_022e63ec *s) {
    Model_Release(s->unk10 + 0x20c);
    MapObject_Destructor(s);
}
