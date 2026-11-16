#include <nitro/types.h>

typedef struct UnkStruct_022dca70 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022dca70;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022dca70 *s);

void Boss4PartD0_Destructor(UnkStruct_022dca70 *s) {
    u8 *p = s->unk10;
    Model_Release(p + 0x20c);
    Model_Release(p + 0x344);
    MapObject_Destructor(s);
}
