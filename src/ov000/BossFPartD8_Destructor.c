#include <nitro/types.h>

typedef struct UnkStruct_022f5678 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022f5678;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022f5678 *s);

void BossFPartD8_Destructor(UnkStruct_022f5678 *s) {
    Model_Release(s->unk10 + 0x470);
    MapObject_Destructor(s);
}
