#include <nitro/types.h>

typedef struct UnkStruct_022f715c {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f715c;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022f715c *s);

void BossFPartDD_Destructor(UnkStruct_022f715c *s) {
    Model_Release(s->unk10 + 0x218);
    MapObject_Destructor(s);
}
