#include <nitro/types.h>

typedef struct UnkStruct_022c74ac {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022c74ac;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022c74ac *s);

void Boss1Hammer_Destructor(UnkStruct_022c74ac *s) {
    Model_Release(s->unk10 + 0x30c);
    MapObject_Destructor(s);
}
