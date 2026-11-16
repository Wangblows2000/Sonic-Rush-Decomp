#include <nitro/types.h>

typedef struct UnkStruct_022f8200 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f8200;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022f8200 *s);

void BossFPartD9_Destructor(UnkStruct_022f8200 *s) {
    Model_Release(s->unk10 + 0x228);
    MapObject_Destructor(s);
}
