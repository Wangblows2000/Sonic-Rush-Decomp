#include <nitro/types.h>

typedef struct UnkStruct_022ec530 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022ec530;

void Sound_FreeHandle(void *p);
void Model_Release(void *p);
void MapObject_Destructor(UnkStruct_022ec530 *s);

void Boss7PartD6_Destructor(UnkStruct_022ec530 *s) {
    u8 *p = s->unk10;
    Sound_FreeHandle(*(void **)(p + 0x384));
    Model_Release(p + 0x20c);
    MapObject_Destructor(s);
}
