#include <nitro/types.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkPlayer_02055fa4;
typedef struct { u8 pad0[4]; UnkPlayer_02055fa4 *player; } UnkA_02055fa4;
typedef struct {
    u8 pad0[8];
    u32 unk8;
    s8 unkC;
    u8 unkD;
    u8 padE[2];
    u16 *col0;  /* 0x10 */
    u16 *col1;  /* 0x14 */
} UnkB_02055fa4;

void func_020560d0(UnkA_02055fa4 *a, UnkB_02055fa4 *b);
void NNS_G3dGlbLightColor(s32 i, u16 v);

#pragma thumb on
void func_02055fa4(UnkA_02055fa4 *a, UnkB_02055fa4 *b) {
    UnkPlayer_02055fa4 *p = a->player;
    u16 *c;

    if (p->flags & 0x4000) {
        p->flags &= ~0x4000;
        func_020560d0(a, b);
        return;
    }
    b->unkC--;
    if (b->unkC <= 0) {
        b->unkC = b->unkD;
        b->unk8 ^= 1;
        if (b->unk8 & 1) {
            c = b->col1;
            NNS_G3dGlbLightColor(0, c[0]);
            NNS_G3dGlbLightColor(1, c[1]);
            NNS_G3dGlbLightColor(2, c[2]);
        } else {
            c = b->col0;
            NNS_G3dGlbLightColor(0, c[0]);
            NNS_G3dGlbLightColor(1, c[1]);
            NNS_G3dGlbLightColor(2, c[2]);
        }
    }
}
#pragma thumb reset
