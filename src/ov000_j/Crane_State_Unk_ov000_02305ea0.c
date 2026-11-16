#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_02305ea0_1e0;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x34 - 0x10];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u8 pad58[0x5c - 0x58];
    u16 unk5C;
    u8 pad5E[0x128 - 0x5e];
    u8 *unk128;
    u8 pad12C[0x1dc - 0x12c];
    void *unk1DC;
    UnkStruct_02305ea0_1e0 *unk1E0;
    u8 pad1E4[0x1f4 - 0x1e4];
    u32 unk1F4;
} UnkStruct_02305ea0;

extern s16 data_0206e470[];

s16 ObjSpd_UpSet(s32 a, s32 b, s32 c);
void Sound_PlaySeqArcEx(void *a, s32 b, s32 c);
void func_02000dfc(void *m);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *out);

void Crane_State_Unk_ov000_02305ea0(UnkStruct_02305ea0 *p) {
    u8 mtx[0x28];
    u8 *obj;
    s32 lim;
    s32 idx;
    s32 v;
    s32 i2;

    obj = p->unk128;
    p->unkC += p->unk34;
    lim = 0x8000;
    if (p->unk54 & 0x1000) {
        if (p->unk1E0->unk4 & 2)
            p->unk34 = ObjSpd_UpSet(p->unk34, 0x12, 0xd00);
        else
            p->unk34 = ObjSpd_UpSet(p->unk34, -0x12, 0xd00);
        if (p->unk1E0->unk4 & 2) {
            if (p->unkC > p->unk8) {
                p->unkC = p->unk8;
                p->unk34 = 0;
                p->unk54 &= ~0x1000;
            }
        } else {
            if (p->unkC < p->unk8) {
                p->unkC = p->unk8;
                p->unk34 = 0;
                p->unk54 &= ~0x1000;
            }
        }
    }
    if (p->unk1F4 & 1) {
        if (p->unk1E0->unk4 & 4)
            lim += 0x10000;
        if (p->unk1E0->unk4 & 2)
            p->unk34 = ObjSpd_UpSet(p->unk34, -0x12, 0xd00);
        else
            p->unk34 = ObjSpd_UpSet(p->unk34, 0x12, 0xd00);
        if (p->unk1E0->unk4 & 2) {
            if (p->unkC < p->unk8 - lim) {
                p->unkC = p->unk8 - 0x8000;
                p->unk5C |= 0x200;
                p->unk54 |= 0x1000;
                p->unk1F4 &= ~1;
                p->unk34 = 0;
                if (p->unk1E0->unk4 & 4)
                    Sound_PlaySeqArcEx(p->unk1DC, 0, 0x40);
            }
        } else {
            if ((u32)p->unkC > (u32)(p->unk8 + lim)) {
                p->unkC = p->unk8 + 0x8000;
                p->unk5C |= 0x200;
                p->unk54 |= 0x1000;
                p->unk1F4 &= ~1;
                p->unk34 = 0;
                if (p->unk1E0->unk4 & 4)
                    Sound_PlaySeqArcEx(p->unk1DC, 0, 0x40);
            }
        }
    }
    func_02000dfc(obj + 0x24);
    MTX_RotY33_(mtx, data_0206e470[(((u16)p->unkC & 0xffff) >> 4) * 2], data_0206e470[(((u16)p->unkC & 0xffff) >> 4) * 2 + 1]);
    func_02000f34(obj + 0x24, mtx, obj + 0x24);
    v = p->unkC & 0xffff;
    if (v < 0xc000 && v < 0x4000) {
        p->unk5C |= 1;
        return;
    }
    p->unk5C &= ~1;
}
