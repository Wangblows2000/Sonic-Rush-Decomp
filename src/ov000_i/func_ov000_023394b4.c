#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x3a - 0xc];
    s8 unk3A;
    u8 pad3B[0x48 - 0x3b];
    s32 unk48;
    u8 pad4C[0x64 - 0x4c];
} UnkPlayer_023394b4;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} UnkObj_023394b4;

typedef struct {
    u8 pad0[8];
    s32 unk8;
} UnkIn_023394b4;

extern UnkPlayer_023394b4 data_ov000_0236aad8[];
extern UnkObj_023394b4 *gPlayers[];

void func_ov000_023393a8(UnkIn_023394b4 *in, s32 idx);
BOOL func_ov000_02338c70(UnkIn_023394b4 *in, s32 idx);
BOOL func_ov000_02338ea0(UnkIn_023394b4 *in, s32 idx);
void func_ov000_023390cc(UnkIn_023394b4 *in, s32 idx);
void func_ov000_02338800(UnkIn_023394b4 *in);

void func_ov000_023394b4(UnkIn_023394b4 *in, s32 idx) {
    UnkPlayer_023394b4 *p = &data_ov000_0236aad8[idx];

    if (p->flags & 0x4020) {
        func_ov000_023393a8(in, idx);
        return;
    }
    if (data_ov000_0236aad8[2].flags & 0x10000) {
        if (!(data_ov000_0236aad8[2].flags & 0x20000)) {
            p->unk8 += in->unk8;
            func_ov000_02338c70(in, idx);
        } else {
            p->unk8 += in->unk8;
            if (in->unk8 != 0 && func_ov000_02338ea0(in, idx)) {
                p->unk8 = gPlayers[p->unk3A]->unk14 - p->unk48;
                data_ov000_0236aad8[2].flags &= ~0x10000;
                data_ov000_0236aad8[2].flags &= ~0x20000;
                func_ov000_02338800(in);
            }
        }
    } else {
        func_ov000_023390cc(in, idx);
    }
    func_ov000_023393a8(in, idx);
}
