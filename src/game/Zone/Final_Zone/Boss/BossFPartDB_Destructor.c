#include <nitro/types.h>

typedef struct UnkStruct_022f7b84 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f7b84;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022f7b84 *s);

void BossFPartDB_Destructor(UnkStruct_022f7b84 *s) {
    Model_Release(s->unk10 + 0x214);
    MapObject_Destructor(s);
}
