#include <nitro/types.h>

typedef struct UnkNode_02022628 UnkNode_02022628;
typedef void (*UnkFunc_02022628)(u32 *, UnkNode_02022628 *, u32);
struct UnkNode_02022628 { u8 unk0[0xC]; UnkFunc_02022628 func; UnkNode_02022628 *next; u8 unk14[6]; u16 unk1A[1]; };

BOOL NNSi_G3dAnmBlendVis(u32 *out, UnkNode_02022628 *n, u32 idx) {
    BOOL r = FALSE;
    u32 tmp[2];
    *out = 0;
    do {
        u16 v = n->unk1A[idx];
        if (v & 0x100) {
            n->func(tmp, n, v & 0xFF);
            *out |= tmp[0];
            r = TRUE;
        }
        n = n->next;
    } while (n != NULL);
    return r;
}
