#include <nitro/types.h>

typedef struct UnkStruct_022dbe38 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022dbe38;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022dbe38 *s);

void Boss4PartD2_Destructor(UnkStruct_022dbe38 *s) {
    u8 *p = s->unk10;
    Model_Release(p + 0x20c);
    Model_Release(p + 0x344);
    MapObject_Destructor(s);
}
