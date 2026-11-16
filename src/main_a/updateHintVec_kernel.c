#include <nitro/types.h>

typedef struct UnkNode_02021dc4 UnkNode_02021dc4;
struct UnkNode_02021dc4 { u8 unk0[0x10]; UnkNode_02021dc4 *next; u8 unk14[5]; u8 count; u16 unk1A[1]; };

void updateHintVec_kernel(u32 *bits, UnkNode_02021dc4 *n) {
    s32 i;
    if (n == NULL) return;
    do {
        for (i = 0; i < n->count; i++) {
            if (n->unk1A[i] & 0x100) {
                bits[i >> 5] |= 1 << (i & 0x1F);
            }
        }
        n = n->next;
    } while (n != NULL);
}
