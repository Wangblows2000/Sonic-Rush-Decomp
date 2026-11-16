#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x20 - 0xc];
    s16 unk20;
    u8 pad22[0x3a - 0x22];
    s8 unk3A;
    u8 pad3B[0x50 - 0x3b];
    s32 unk50;
    u8 pad54[0x58 - 0x54];
    s32 unk58;
    u8 pad5C[0x64 - 0x5c];
} UnkPlayer_02338c70;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} UnkObj_02338c70;

extern UnkPlayer_02338c70 data_ov000_0236aad8[];
extern UnkObj_02338c70 *data_ov000_0236ac60[];

BOOL func_ov000_02338c70(u32 *in, s32 idx) {
    UnkPlayer_02338c70 *p = &data_ov000_0236aad8[idx];
    s32 lim;
    u32 f;
    lim = data_ov000_0236ac60[p->unk3A]->unk14;
    f = data_ov000_0236aad8[2].flags & 0x20000;

    if ((!f && (*in & 2)) || (f && (*in & 4))) {
        if (p->unk8 <= p->unk50) {
            p->unk8 = p->unk50;
            return TRUE;
        }
        lim -= p->unk20 * 0xa8;
        if (p->unk8 <= lim) {
            p->unk8 = lim;
            return TRUE;
        }
    } else {
        if (p->unk8 >= p->unk58) {
            p->unk8 = p->unk58;
            return TRUE;
        }
        lim -= p->unk20 * 0x18;
        if (p->unk8 >= lim) {
            p->unk8 = lim;
            return TRUE;
        }
    }
    return FALSE;
}
