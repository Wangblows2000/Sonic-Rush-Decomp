#include <nitro/types.h>

typedef struct UnkStruct_02307e84_2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_02307e84_2;

typedef struct UnkStruct_02307e84 {
    u8 pad[0x1e0];
    UnkStruct_02307e84_2 *unk1E0;
} UnkStruct_02307e84;

u8 func_ov000_02307e84(UnkStruct_02307e84 *s) {
    return (s->unk1E0->unk4 & 0xe0) >> 5;
}
