#include <nitro/types.h>

typedef struct { u8 unk0[0x2C]; u32 unk2C; u16 unk30; u16 unk32; } UnkStruct_02021a44;

u32 NNS_G3dPlttReleasePlttKey(UnkStruct_02021a44 *s) {
    u32 r;
    s->unk32 &= ~1;
    r = s->unk2C;
    s->unk2C = 0;
    return r;
}
