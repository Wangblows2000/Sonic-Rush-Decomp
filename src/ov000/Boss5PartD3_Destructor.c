#include <nitro/types.h>

typedef struct UnkStruct_022ca550 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022ca550;

void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022ca550 *s);

void Boss5PartD3_Destructor(UnkStruct_022ca550 *s) {
    Model_Release(s->unk10 + 0x20c);
    MapObject_Destructor(s);
}
