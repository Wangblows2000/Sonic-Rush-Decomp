#include <nitro/types.h>

typedef struct UnkStruct_0230c370 {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad1[0x2c];
    u16 unk5C;
    u8 pad2[0xa];
    void (*func)(void);
} UnkStruct_0230c370;

void MapObj13_State_Unk_ov000_0230c2d0(void);

void func_ov000_0230c370(UnkStruct_0230c370 *s) {
    s->unk5C |= 0x20;
    s->func = MapObj13_State_Unk_ov000_0230c2d0;
    s->unk2E = -0x800;
}
