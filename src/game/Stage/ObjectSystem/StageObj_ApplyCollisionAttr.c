#include <nitro/types.h>

typedef struct {
    u8 pad0[0x5e];
    u16 unk5e;
} UnkPlayer_020660b0;

void StageObj_ApplyCollisionAttr(UnkPlayer_020660b0 *p, u8 attr) {
    u8 a = attr >> 1;
    if (a == 1)
        p->unk5e |= 1;
    if (a == 3)
        p->unk5e |= 0x10;
    if (a == 2 || a == 3)
        p->unk5e |= 8;
}
