#include <nitro/types.h>

typedef struct { u8 unk0[0xC]; u16 unkC; } UnkStruct_02021c18;

u32 NNS_G3dTexGetRequiredSize(UnkStruct_02021c18 *s) {
    return s->unkC << 3;
}
