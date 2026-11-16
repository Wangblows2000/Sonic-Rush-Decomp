#include <nitro/types.h>

typedef struct UnkStruct_0230bc38 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad2[4];
    u16 unk5C;
} UnkStruct_0230bc38;

void MapObj13Part_State_DestroyOnAnimEnd(UnkStruct_0230bc38 *s) {
    if (s->unk5C & 8)
        s->unk54 |= 4;
}
