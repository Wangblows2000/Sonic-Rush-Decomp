#include <nitro/types.h>

typedef struct UnkNode_02326ac8 {
    s32 unk0;
    u8 pad4[8];
    s32 unkC;
    u8 pad10[8];
    struct UnkNode_02326ac8 *next;
} UnkNode_02326ac8;

typedef struct {
    u8 pad0[0x29c];
    UnkNode_02326ac8 *unk29C;
} UnkWork_02326ac8;

extern u8 gEffTrailWrapPending;
extern s32 gEffTrailWrapShift;

void EffTrail_ApplyWrapShift(UnkWork_02326ac8 *w) {
    s32 i;
    UnkNode_02326ac8 *n = w->unk29C;

    if (gEffTrailWrapPending == 0)
        return;
    for (i = 0; i < 11; i++) {
        n->unk0 += gEffTrailWrapShift;
        n->unkC += gEffTrailWrapShift;
        n = n->next;
    }
    gEffTrailWrapPending = 0;
}
