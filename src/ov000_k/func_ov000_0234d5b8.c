#include <nitro/types.h>

typedef struct {
    u8 pad0[0x42C];
    u16 unk42C;
    u16 unk42E;
    u16 unk430;
    u16 unk432;
    u16 unk434;
    u16 unk436;
} UnkStruct_0234d5b8;

u16 func_ov000_0234d5b8(UnkStruct_0234d5b8 *w, u16 in) {
    u16 out = in & ~0xF3;
    if (in & 0x10) out |= w->unk42C;
    if (in & 0x20) out |= w->unk42E;
    if (in & 0x40) out |= w->unk430;
    if (in & 0x80) out |= w->unk432;
    if (in & 1) out |= w->unk434;
    if (in & 2) out |= w->unk436;
    return out;
}
