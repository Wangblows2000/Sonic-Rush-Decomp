#include <nitro/types.h>

typedef struct UnkStruct_0231fdec {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_0231fdec;

extern u32 gGameState[];

void EffHahenZako_State_Main(UnkStruct_0231fdec *s) {
    if (gGameState[6] & 1)
        s->unk5C &= ~0x20;
}
