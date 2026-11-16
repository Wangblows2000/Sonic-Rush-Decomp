#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20c];
    u32 unk20C;
    u16 unk210;
} UnkStruct_022f8224;

void BossF_Unk_ov000_022f8a48(void);

/* Explicit form of CW's binary-search lowering of switch (timer & 0xf) { 0, 4, 8, 12 }. */
void BossF_State_Unk_ov000_022f8224(UnkStruct_022f8224 *p)
{
    s32 t;

    p->unk210++;
    t = p->unk210 & 0xf;
    if (t <= 8) {
        if (t >= 8)
            goto case8;
        if (t > 4)
            return;
        if (t < 0)
            return;
        if (t == 0)
            goto case0;
        if (t == 4)
            goto case4;
        return;
    }
    if (t == 12)
        goto case4;
    return;
case0:
    if (p->unk20C & 2)
        BossF_Unk_ov000_022f8a48();
    return;
case8:
    if (p->unk20C & 4)
        BossF_Unk_ov000_022f8a48();
    return;
case4:
    if (p->unk20C & 8)
        BossF_Unk_ov000_022f8a48();
}
