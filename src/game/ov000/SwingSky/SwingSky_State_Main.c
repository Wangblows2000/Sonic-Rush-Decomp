#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x3E8 - 1];
    void *unk3E8;
    u8 pad3EC[0x420 - 0x3EC];
    u16 unk420;
    u16 unk422;
} UnkStruct_02317e50_P;

typedef struct {
    u8 pad0[8];
    u32 unk8;
    s32 unkC;
    u8 pad10[0x28 - 0x10];
    s16 unk28;
    s16 unk2A;
    u8 pad2C[0x34 - 0x2C];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u8 pad58[0x1E0 - 0x58];
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x1F4 - 0x1E4];
    u32 unk1F4;
    u8 pad1F8[0x204 - 0x1F8];
    UnkStruct_02317e50_P *unk204;
    u8 pad208[0x4A2 - 0x208];
    s16 unk4A2;
} UnkStruct_02317e50;

s32 ObjSpd_AccelToward(void *w, s32 v, s32 a, s32 b, s32 c);

void SwingSky_State_Main(UnkStruct_02317e50 *w) {
    UnkStruct_02317e50_P *p;
    s32 old = w->unkC;
    s16 a = 0x2a;
    s16 b = 10;
    s32 v;

    if (!(w->unk1F4 & 1))
        return;
    p = w->unk204;
    if (p != NULL && p->unk0 == 1) {
        if (w->unk34 != 0)
            w->unk4A2 = 0x400;
        if (p->unk3E8 != w) {
            w->unk204 = NULL;
            w->unk8 = 0;
        } else {
            if (p->unk420 & 0x30) {
                if ((w->unk34 < 0 && (p->unk420 & 0x10)) || (w->unk34 > 0 && (p->unk420 & 0x20))) {
                    b = 0;
                    if (w->unkC > 0 && w->unk34 > 0)
                        a += 8;
                    if (w->unkC < 0 && w->unk34 < 0)
                        a += 8;
                }
                if ((w->unk34 > 0 && (p->unk420 & 0x10)) || (w->unk34 < 0 && (p->unk420 & 0x20)))
                    b += 8;
            }
            v = w->unk34;
            if (v < 0)
                v = -v;
            if (v < 0x1c0) {
                v = w->unkC;
                if (v < 0)
                    v = -v;
                if (v < 0x1000) {
                    if (p->unk422 & 0x10)
                        w->unk34 = -0x300;
                    else if (p->unk422 & 0x20)
                        w->unk34 = 0x300;
                }
            }
        }
    } else if (w->unk54 & 2) {
        w->unk8++;
        if (w->unk8 > 0x18) {
            w->unk54 &= ~2;
            w->unk8 = 0;
        }
    }
    w->unkC = ObjSpd_AccelToward(w, w->unkC, a, b, 0xf00);
    if ((old > 0 && w->unkC <= 0) || (old < 0 && w->unkC >= 0))
        w->unk2A = w->unk34;
    w->unk28 = w->unk34;
    if (w->unk1E0->unk8 > 0x40 && w->unk1E0->unk8 < 0xc0)
        return;
    if (w->unkC > 0x4800)
        w->unkC = 0x4800;
    if (w->unkC < -0x4800)
        w->unkC = -0x4800;
}
