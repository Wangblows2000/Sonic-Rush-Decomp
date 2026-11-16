#include <nitro/types.h>

typedef struct {
    s16 unk0;
    u8 pad2[0x30 - 2];
} UnkStruct_02368baa;

extern UnkStruct_02368baa data_ov000_02368baa[];

void func_ov000_023460ec(u8 *w) {
    if (*(s16 *)(w + 0x328) == 0xc || (*(u32 *)(w + 0x32c) & 0x80)) {
        *(u32 *)(w + 0x58) |= 0x2000000;
        if (*(u32 *)(w + 0x32c) & 0x80) {
            *(s16 *)(w + 0x48) = *(s16 *)(w + 0x368) >> 1;
            return;
        }
        *(s16 *)(w + 0x48) = data_ov000_02368baa[w[0x324]].unk0;
        return;
    }
    *(s16 *)(w + 0x48) = data_ov000_02368baa[w[0x324]].unk0;
    *(u32 *)(w + 0x58) &= ~0x2000000;
}
