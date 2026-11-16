#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x58 - 1];
    u32 unk58;
    u16 unk5C;
} UnkStruct_02318e68_P;

typedef struct {
    u8 pad0[4];
    UnkStruct_02318e68_P *unk4;
    UnkStruct_02318e68_P *unk8;
} UnkStruct_02318e68_L;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0x2E - 0x10];
    s16 unk2E;
    u8 pad30[0x36 - 0x30];
    s16 unk36;
    u8 pad38[0x58 - 0x38];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x130 - 0x5E];
    UnkStruct_02318e68_L *unk130;
    u8 pad134[0x1E0 - 0x134];
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x1F4 - 0x1E4];
    u32 unk1F4;
} UnkStruct_02318e68;

extern s16 data_ov000_0236a2bc[];

void func_ov000_02318ba0(s32 a, u16 b);
s32 func_02068ce4(s32 v, s32 a, s32 b, s32 c, s32 d);

void func_ov000_02318e68(UnkStruct_02318e68 *w) {
    s32 flag = 0;
    UnkStruct_02318e68_P *p;
    s32 d;
    s32 old;

    w->unk2E = 0;
    p = w->unk130->unk8;
    if (p != NULL && p->unk0 == 1 && w->unk130->unk4 != p && (p->unk58 & 4) &&
        (((w->unk5C & 1) && (p->unk5C & 1)) || (!(w->unk5C & 1) && !(p->unk5C & 1)))) {
        if (data_ov000_0236a2bc[w->unk1E0->unk6] == 0)
            func_ov000_02318ba0(w->unk1E0->unk6, w->unk1E0->unk4 & 2);
        flag = 1;
        data_ov000_0236a2bc[w->unk1E0->unk6] = (w->unk1E0->unk8 + w->unk1E0->unk9) * 2;
        w->unk1F4 |= 2;
    }
    if (w->unk1F4 & 1) {
        w->unk2E = -0x100;
        if (w->unk5C & 1)
            w->unk2E = -w->unk2E;
        d = w->unkC;
        if (d < 0)
            d = -d;
        if (d <= 0x200)
            w->unk2E = 0;
    } else if (w->unk1F4 & 2) {
        w->unk2E = 0x80;
        if (w->unk5C & 1)
            w->unk2E = -w->unk2E;
        d = w->unkC;
        if (d < 0)
            d = -d;
        if (d >= 0x400) {
            w->unk2E = 0;
            w->unk1F4 |= 1;
            w->unk1F4 &= ~2;
        }
    }
    if (!flag && data_ov000_0236a2bc[w->unk1E0->unk6] == 0) {
        old = w->unkC;
        w->unk1F4 &= ~1;
        w->unkC = func_02068ce4(w->unkC, 0, 2, 0x200, 0x80);
        w->unk2E = w->unkC - old;
        w->unk58 &= ~0x4000000;
        return;
    }
    w->unkC += w->unk2E + w->unk36;
}
