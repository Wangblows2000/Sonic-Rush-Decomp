#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[4];
    s32 unk8;
    u8 padC[0x4C - 0xC];
    s32 unk4C[4];
    s8 unk5C[4];
    u8 pad60[0x64 - 0x60];
} UnkStruct_Plr;

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_Pad;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C[2];
    s32 unk214[2][4];
    void *unk234[2];
} UnkStruct_02302f54;

extern UnkStruct_Plr data_ov000_0236aad8[];

void func_ov000_02302f18(void);

void func_ov000_02302f54(UnkStruct_02302f54 *w, s32 i) {
    s32 v;
    u32 g;

    w->unk20C[i] |= 2;
    if (((UnkStruct_Pad *)w->base.unk1E0)->unk4 & 1) {
        data_ov000_0236aad8[i].unk4C[0] = w->unk214[i][0];
        w->unk20C[i] |= 4;
        data_ov000_0236aad8[i].unk5C[0]++;
    }
    if (((UnkStruct_Pad *)w->base.unk1E0)->unk4 & 4) {
        data_ov000_0236aad8[i].unk4C[2] = w->unk214[i][2];
        w->unk20C[i] |= 0x10;
        data_ov000_0236aad8[i].unk5C[2]++;
    }
    g = data_ov000_0236aad8[2].unk0;
    if (g & 1) {
        if (g & 2) {
            if (((UnkStruct_Pad *)w->base.unk1E0)->unk4 & 2) {
                v = w->unk214[i][1];
                if (data_ov000_0236aad8[i].unk8 - 0x11000 >= v) {
                    data_ov000_0236aad8[i].unk4C[1] = v + 0x11000;
                } else {
                    data_ov000_0236aad8[i].unk0 |= 0x20;
                    w->unk20C[i] |= 0x80;
                }
                w->unk20C[i] |= 8;
                data_ov000_0236aad8[i].unk5C[1]++;
            }
            if (((UnkStruct_Pad *)w->base.unk1E0)->unk4 & 8) {
                v = w->unk214[i][3];
                if (data_ov000_0236aad8[i].unk8 + 0xc000 <= v) {
                    data_ov000_0236aad8[i].unk4C[3] = v;
                } else {
                    data_ov000_0236aad8[i].unk0 |= 0x20;
                    w->unk20C[i] |= 0x80;
                }
                w->unk20C[i] |= 0x20;
                data_ov000_0236aad8[i].unk5C[3]++;
            }
        } else {
            if (((UnkStruct_Pad *)w->base.unk1E0)->unk4 & 2) {
                v = w->unk214[i][1];
                if (data_ov000_0236aad8[i].unk8 >= v) {
                    data_ov000_0236aad8[i].unk4C[1] = v;
                } else {
                    data_ov000_0236aad8[i].unk0 |= 0x20;
                    w->unk20C[i] |= 0x80;
                }
                w->unk20C[i] |= 8;
                data_ov000_0236aad8[i].unk5C[1]++;
            }
            if (((UnkStruct_Pad *)w->base.unk1E0)->unk4 & 8) {
                v = w->unk214[i][3];
                if (data_ov000_0236aad8[i].unk8 + 0x1d000 <= v) {
                    data_ov000_0236aad8[i].unk4C[3] = v - 0x11000;
                } else {
                    data_ov000_0236aad8[i].unk0 |= 0x20;
                    w->unk20C[i] |= 0x80;
                }
                w->unk20C[i] |= 0x20;
                data_ov000_0236aad8[i].unk5C[3]++;
            }
        }
    } else {
        if (((UnkStruct_Pad *)w->base.unk1E0)->unk4 & 2) {
            data_ov000_0236aad8[i].unk4C[1] = w->unk214[i][1];
            w->unk20C[i] |= 8;
            data_ov000_0236aad8[i].unk5C[1]++;
        }
        if (((UnkStruct_Pad *)w->base.unk1E0)->unk4 & 8) {
            data_ov000_0236aad8[i].unk4C[3] = w->unk214[i][3];
            w->unk20C[i] |= 0x20;
            data_ov000_0236aad8[i].unk5C[3]++;
        }
    }
    w->unk234[i] = func_ov000_02302f18;
}
