#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x10 - 4];
    s32 unk10;
    u8 pad14[0x18 - 0x14];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
    u32 unk20;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x40 - 0x14];
    s32 unk40;
    u8 pad44[2];
    u8 unk46;
    u8 unk47;
    u32 unk48;
    u32 unk4C;
    void *unk50;
    void *unk54;
} UnkStruct_022c4590;

typedef struct {
    u8 pad0[0x28];
    s16 unk28;
    u8 pad2A[0x32C - 0x2A];
    u32 unk32C;
    u8 pad330[0x37C - 0x330];
    s16 unk37C;
    u8 pad37E[0x3AE - 0x37E];
    s16 unk3AE;
} UnkStruct_Player;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_Player *gPlayers;

s32 func_02002094(s32 a, s32 b);
void MIi_CpuCopy16(void *a, void *b, s32 c);
void func_ov000_02347c6c(UnkStruct_Player *p, void *a);
void func_ov000_022fa9e8(void *a, UnkStruct_Player *p, s32 b);
void func_ov000_023422e4(UnkStruct_Player *p);

void func_ov000_0230ad04(void *a, UnkStruct_Player *p) {
    s32 v;

    if (p->unk32C & 0x80) {
        data_022c4590.unk8 += 10000;
    } else if (p->unk32C & 0x100) {
        data_022c4590.unk8 += 8000;
    } else {
        v = p->unk28;
        if (v < 0)
            v = -v;
        if (p->unk37C <= v)
            data_022c4590.unk8 += 4000;
    }
    data_022c4590.unk8 = func_02002094(data_022c4590.unk8, ++data_022c4590.unkC);
    data_022c4590.unk10 = p->unk3AE * 10;
    if (data_022c4560.unk10 == 2) {
        if (data_022c4590.unk4C != 0 && data_022c4590.unk46 == data_022c4560.unk1C &&
            data_022c4590.unk47 == data_022c4560.unk1D) {
            if (data_022c4590.unk4C > data_022c4560.unk20) {
                data_022c4590.unk4C = data_022c4560.unk20;
                data_022c4590.unk48 = data_022c4560.unk0;
                MIi_CpuCopy16(data_022c4590.unk54, data_022c4590.unk50, 0x3138);
            }
        } else {
            data_022c4590.unk46 = data_022c4560.unk1C;
            data_022c4590.unk47 = data_022c4560.unk1D;
            data_022c4590.unk4C = data_022c4560.unk20;
            data_022c4590.unk48 = data_022c4560.unk0;
            MIi_CpuCopy16(data_022c4590.unk54, data_022c4590.unk50, 0x3138);
        }
    }
    data_022c4560.unk18 &= ~1;
    func_ov000_02347c6c(p, a);
    if (data_022c4560.unk10 == 1) {
        if (p != gPlayers) {
            data_022c4590.unk40 = 2;
        } else {
            data_022c4590.unk40 = 1;
            func_ov000_022fa9e8(a, p, 3);
        }
    }
    func_ov000_023422e4(p);
}
