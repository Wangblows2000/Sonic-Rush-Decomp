#include <nitro/types.h>

typedef struct UnkStruct_022ec138 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022ec138;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022ec138 *s);

void Boss7PartD7_Destructor(UnkStruct_022ec138 *s) {
    Model_Release(s->unk10 + 0x20c);
    MapObject_Destructor(s);
}
