#include <nitro/types.h>

typedef struct UnkStruct_023099c0_2 {
    u8 pad[0x214];
    u8 unk214;
} UnkStruct_023099c0_2;

typedef struct UnkStruct_023099c0 {
    u8 pad[0x10];
    UnkStruct_023099c0_2 *unk10;
} UnkStruct_023099c0;

BOOL EnemyRoom_Unregister(s32 a, s32 i);
void MapObject_Destructor(UnkStruct_023099c0 *s);

void MapObj8C_Destructor(UnkStruct_023099c0 *s) {
    EnemyRoom_Unregister(1, s->unk10->unk214);
    MapObject_Destructor(s);
}
