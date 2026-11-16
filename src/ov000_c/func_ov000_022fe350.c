#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 unk20C[0x23A - 0x20C];
    s16 unk23A;
    s16 unk23C;
} UnkStruct_022fe350;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_0236aad8 data_ov000_0236aad8;
extern UnkStruct_022c4560 data_022c4560;

void func_ov000_022fe304(UnkStruct_022fe350 *w);
void func_ov000_022fe204(UnkStruct_022fe350 *w);
void func_ov000_022fdfd4(UnkStruct_022fe350 *w);
s16 ObjSpd_DownSet(s32 a, s32 b);
s16 ObjSpd_UpSet(s32 a, s32 b, s32 c);
void StageObj_HandleCollider(void *w, void *a);

static inline u16 FrameRand(void) {
    return (u16)(((data_022c4560.unk20 << 16) * 0x196225 + 0x3c6ef35f) >> 16);
}

void func_ov000_022fe350(UnkStruct_022fe350 *w) {
    s32 y = w->base.unk1E8;
    s16 spd = 0x100;
    u16 k;
    u32 flags;
    s32 d;
    UnkStruct_StageObj *parent;

    if (w->unk23A != 0) {
        if (data_ov000_0236aad8.unk62 != 0xffff) {
            w->base.unk58 &= ~0x100;
            w->base.unk58 &= ~0x80;
            w->base.unk30 = 0;
        }
        parent = w->base.unk80;
        if (parent != NULL)
            y = (s16)((parent->unk10 >> 8) + w->unk23A);
        w->unk23C++;
    }
    k = w->base.unk1E0->unk2;
    if (k == 7 || k == 0x15)
        spd -= 0x40;
    if (k == 8)
        spd -= 0x80;
    if (k == 0xf)
        spd += 0x300;
    flags = w->base.unk1F4;
    if (flags & 4) {
        w->base.unkC--;
        if (w->base.unkC == 0) {
            w->base.unk1F4 &= ~4;
            w->base.unkC = 200;
            func_ov000_022fe304(w);
            return;
        }
    } else if (flags & 1) {
        w->base.unkC--;
        if (w->base.unkC == 0) {
            w->base.unk1F4 &= ~1;
            w->base.unkC = (FrameRand() & 0x1f) + 0x40;
            w->base.unk1F4 |= 4;
        }
    } else if (w->base.unkC != 0) {
        w->base.unkC--;
    } else if (!(FrameRand() & 0x1ff) && k != 8) {
        switch (k) {
        case 7:
        case 0xd:
        case 0x15:
            if (w->unk23C <= 200 && w->base.unk8 == 0) {
                func_ov000_022fe204(w);
                return;
            }
            break;
        case 0xa:
            func_ov000_022fdfd4(w);
            return;
        case 0xe:
        case 0xf:
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        default:
            w->base.unk1F4 |= 1;
            func_ov000_022fe304(w);
            return;
        }
    }
    if (w->base.unk1E0->unk4 & 3)
        spd = spd + (((w->base.unk1E0->unk4 & 0xc) >> 2) << 7);
    if (w->base.unk1F4 & 5) {
        w->base.unk34 = ObjSpd_DownSet(w->base.unk34, (spd << 6) >> 8);
        StageObj_HandleCollider(w, w->unk20C);
        return;
    }
    if (w->base.unk5C & 1)
        w->base.unk34 = ObjSpd_UpSet(w->base.unk34, -((spd * 10) >> 8), (spd * 0xc0) >> 8);
    else
        w->base.unk34 = ObjSpd_UpSet(w->base.unk34, (spd * 10) >> 8, (spd * 0xc0) >> 8);
    if (w->unk23C <= 200 || !(w->base.unk5C & 1)) {
        d = w->base.unk28 - w->base.unk36;
        if (d > 0 && w->base.unk10 > (w->base.unk1E0->unk8 + (y + w->base.unk1E0->unk6)) << 8) {
            func_ov000_022fe304(w);
            return;
        }
        if (d < 0 && w->base.unk10 < (y + w->base.unk1E0->unk6) << 8) {
            func_ov000_022fe304(w);
            return;
        }
    }
    if (w->base.unk58 & 4) {
        func_ov000_022fe304(w);
        return;
    }
    if (w->unk23C <= 200) {
        if (w->base.unk8 != 0)
            w->base.unk8--;
        else
            StageObj_HandleCollider(w, w->unk20C);
    }
    if (w->base.unk1E0->unk2 == 0xf && (w->base.spr0.unkE >> 4) > 0x13)
        w->base.unk34 = 0;
    if (w->base.unk58 & 1)
        return;
    if (w->base.unk58 & 0x80)
        w->base.unk34 = 0;
}
