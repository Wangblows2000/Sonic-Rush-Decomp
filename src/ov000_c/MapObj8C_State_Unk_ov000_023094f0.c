#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x34 - 4];
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkStruct_Plr;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[2];
    s8 unk212;
    s8 unk213;
    u8 unk214;
} UnkStruct_023094f0;

extern UnkStruct_Plr data_ov000_0236aad8[];

BOOL func_ov000_02307dfc(UnkStruct_023094f0 *w, s32 a);
void func_ov000_0234232c(s32 a);
void func_ov000_02309430(UnkStruct_023094f0 *w);

void MapObj8C_State_Unk_ov000_023094f0(UnkStruct_023094f0 *w) {
    u8 i;
    BOOL all;
    u32 f;

    if (func_ov000_02307dfc(w, 1)) {
        if (data_ov000_0236aad8[2].unk0 & 1) {
            data_ov000_0236aad8[0].unk34 &= ~0x10000;
            data_ov000_0236aad8[1].unk34 &= ~0x10000;
        } else {
            if (w->unk20C & 2)
                data_ov000_0236aad8[0].unk34 &= ~0x10000;
            if (w->unk20C & 4)
                data_ov000_0236aad8[1].unk34 &= ~0x10000;
        }
        func_ov000_0234232c(w->unk214);
        w->base.unk1F4 &= ~0x10;
        w->base.unk54 |= 4;
        return;
    }
    f = w->unk20C;
    if (!(f & 0x1000000)) {
        all = TRUE;
        for (i = 0; i < 2; i = (u8)(i + 1)) {
            if ((f & (2 << i)) && (0x550000 << i) != (f & (0x550000 << i))) {
                all = FALSE;
                break;
            }
        }
        if (all)
            w->unk20C |= 0x1000000;
    }
    if ((w->unk20C & 0x1000020) == 0x1000020)
        w->unk20C |= 0x100;
    if (w->unk213 < w->unk212)
        return;
    func_ov000_02309430(w);
}
