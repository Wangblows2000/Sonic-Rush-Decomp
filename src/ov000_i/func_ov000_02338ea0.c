#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x3a - 0xc];
    s8 unk3A;
    u8 pad3B[0x48 - 0x3b];
    s32 unk48;
    u8 pad4C[0x64 - 0x4c];
} UnkPlayer_02338ea0;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} UnkObj_02338ea0;

typedef struct {
    u8 pad0[8];
    s32 unk8;
} UnkIn_02338ea0;

extern UnkPlayer_02338ea0 data_ov000_0236aad8[];
extern UnkObj_02338ea0 *data_ov000_0236ac60[];

BOOL func_ov000_02338ea0(UnkIn_02338ea0 *in, s32 idx) {
    UnkPlayer_02338ea0 *p = &data_ov000_0236aad8[idx];

    if (in->unk8 >= 0)
        return p->unk8 + p->unk48 >= data_ov000_0236ac60[p->unk3A]->unk14;
    return p->unk8 + p->unk48 <= data_ov000_0236ac60[p->unk3A]->unk14;
}
