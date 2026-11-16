#include <nitro/types.h>

typedef struct { u8 unk0[0x1C]; u16 unk1C; } UnkStruct_02021c0c;

u32 NNS_G3dTex4x4GetRequiredSize(UnkStruct_02021c0c *s) {
    return s->unk1C << 3;
}
