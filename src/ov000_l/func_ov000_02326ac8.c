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

extern u8 data_ov000_0236a970;
extern s32 data_ov000_0236a974;

void func_ov000_02326ac8(UnkWork_02326ac8 *w) {
    s32 i;
    UnkNode_02326ac8 *n = w->unk29C;

    if (data_ov000_0236a970 == 0)
        return;
    for (i = 0; i < 11; i++) {
        n->unk0 += data_ov000_0236a974;
        n->unkC += data_ov000_0236a974;
        n = n->next;
    }
    data_ov000_0236a970 = 0;
}
