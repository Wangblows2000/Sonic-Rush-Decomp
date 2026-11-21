#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x10 - 1];
    s32 unk10;
    u8 pad14[0x58 - 0x14];
    u32 unk58;
    u8 pad5C[0x64 - 0x5C];
    s8 unk64;
    u8 pad65;
    s8 unk66;
} UnkStruct_0231907c_P;

typedef struct {
    u8 pad0[4];
    UnkStruct_0231907c_P *unk4;
    UnkStruct_0231907c_P *unk8;
} UnkStruct_0231907c_L;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    u8 pad14[0x30 - 0x14];
    s16 unk30;
    u8 pad32[0x5C - 0x32];
    u16 unk5C;
    u8 pad5E[0x130 - 0x5E];
    UnkStruct_0231907c_L *unk130;
    u8 pad134[0x1E0 - 0x134];
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x1F4 - 0x1E4];
    u32 unk1F4;
} UnkStruct_0231907c;

extern s16 gSwitchTimers[];

void SwitchTimer_Create(s32 a, u16 b);
s32 ObjShift_Set(s32 v, s32 a, s32 b, s32 c, s32 d);

void Switch_State_Vertical(UnkStruct_0231907c *w) {
    s32 flag = 0;
    UnkStruct_0231907c_P *p;
    s32 d;
    s32 old;

    w->unk30 = 0;
    p = w->unk130->unk8;
    if (p != NULL && p->unk0 == 1) {
        if (w->unk130->unk4 == p || ((p->unk58 & 2) && (w->unk5C & 2))) {
            d = (w->unk10 >> 8) - ((p->unk10 >> 8) + p->unk64);
            if (d < 0)
                d = -d;
            if (d < 14)
                goto hit;
        }
        d = (w->unk10 >> 8) - (p->unk66 + (p->unk10 >> 8));
        if (d < 0)
            d = -d;
        if (d < 14) {
        hit:
            if (gSwitchTimers[w->unk1E0->unk6] == 0)
                SwitchTimer_Create(w->unk1E0->unk6, w->unk1E0->unk4 & 2);
            flag = 1;
            gSwitchTimers[w->unk1E0->unk6] = (w->unk1E0->unk8 + w->unk1E0->unk9) * 2;
            w->unk1F4 |= 2;
            w->unk1F4 &= ~1;
        }
    }
    if (w->unk1F4 & 1) {
        w->unk30 = -0x100;
        if (w->unk5C & 2)
            w->unk30 = -w->unk30;
        if (w->unkC <= 0x200)
            w->unk30 = 0;
    } else if (w->unk1F4 & 2) {
        w->unk30 = 0x80;
        if (w->unk5C & 2)
            w->unk30 = -w->unk30;
        if (w->unkC >= 0x400) {
            w->unk30 = 0;
            w->unk1F4 |= 1;
            w->unk1F4 &= ~2;
        }
    }
    if (!flag && gSwitchTimers[w->unk1E0->unk6] == 0) {
        old = w->unkC;
        w->unk1F4 &= ~1;
        w->unkC = ObjShift_Set(w->unkC, 0, 2, 0x200, 0x80);
        w->unk30 = w->unkC - old;
        if (w->unk5C & 2)
            w->unk30 = -w->unk30;
        return;
    }
    if (w->unk5C & 2)
        w->unkC -= w->unk30;
    else
        w->unkC += w->unk30;
}
