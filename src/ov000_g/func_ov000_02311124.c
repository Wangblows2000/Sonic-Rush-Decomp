#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    s32 unk20C;
    u8 pad210[0x228 - 0x210];
    u16 unk228;
    u8 pad22A[2];
    u16 unk22C;
    u16 unk22E;
} UnkStruct_02311124;

extern void *gPlayers;
extern s16 gSwitchTimers[];

void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Sound_SetPositional(void *a, void *b, void *c);
void func_ov000_02311684(UnkStruct_02311124 *w);
void func_02067d58(void *a);

void func_ov000_02311124(UnkStruct_02311124 *w) {
    UnkStruct_ObjParam *p;
    u32 n;
    u16 c;

    w->base.unk5C &= ~0x20;
    if (w->base.unk1E0->unk2 == 0xc0) {
        if (w->base.unk1E0->unk7 > 0) {
            if (w->base.unk10 > (((u32)w->base.unk8 + 1) * 12 + w->base.unk1E8) << 8) {
                w->base.unk8++;
                Sound_PlaySeqArcEx((void *)w->base.unk1DC, 0, 0x70);
            }
            if (w->base.unk10 > (w->base.unk1E0->unk8 * 12 + w->base.unk1E8) << 8) {
                w->base.unk10 = w->base.unk1E8 << 8;
                w->base.unk8 = 0;
            }
        } else {
            if (w->base.unk10 < (w->base.unk1E8 - ((u32)w->base.unk8 + 1) * 12) << 8) {
                w->base.unk8++;
                Sound_PlaySeqArcEx((void *)w->base.unk1DC, 0, 0x70);
            }
            if (w->base.unk10 < (w->base.unk1E8 - w->base.unk1E0->unk8 * 12) << 8) {
                w->base.unk10 = w->base.unk1E8 << 8;
                w->base.unk8 = 0;
            }
        }
    } else {
        if (w->base.unk1E0->unk7 > 0) {
            if (w->base.unk14 > (((u32)w->base.unk8 + 1) * 12 + w->base.unk1EC) << 8) {
                w->base.unk8++;
                Sound_PlaySeqArcEx((void *)w->base.unk1DC, 0, 0x70);
            }
            if (w->base.unk14 > (w->base.unk1E0->unk8 * 12 + w->base.unk1EC) << 8) {
                w->base.unk14 = w->base.unk1EC << 8;
                w->base.unk8 = 0;
            }
        } else {
            if (w->base.unk14 < (w->base.unk1EC - ((u32)w->base.unk8 + 1) * 12) << 8) {
                w->base.unk8++;
                Sound_PlaySeqArcEx((void *)w->base.unk1DC, 0, 0x70);
            }
            if (w->base.unk14 < (w->base.unk1EC - w->base.unk1E0->unk8 * 12) << 8) {
                w->base.unk14 = w->base.unk1EC << 8;
                w->base.unk8 = 0;
            }
        }
    }
    Sound_SetPositional((void *)w->base.unk1DC, w, gPlayers);
    p = w->base.unk1E0;
    if (p->unk6 != 0 && gSwitchTimers[p->unk6] == 0) {
        func_ov000_02311684(w);
        return;
    }
    if (p->unk2 == 0xc0) {
        c = p->unk9 * 2;
        if (p->unk7 > 0) {
            if ((u32)w->base.unk8 < p->unk9 * 2)
                c = w->base.unk8 + 1;
            w->base.unk1D8 = (c * 12) >> 3;
            w->unk22C = ((w->base.unk1E0->unk9 * 2 - c) * 12) >> 3;
            w->unk228 = w->base.unk1D4 - w->base.unk1E0->unk8 * 12 + w->base.unk1D8 * 8;
        } else {
            if ((u32)w->base.unk8 < p->unk9 * 2)
                c = w->base.unk8 + 1;
            w->base.unk1D8 = (c * 12) >> 3;
            w->base.unk1D4 = -(w->base.unk1D8 << 3);
            w->unk22C = ((w->base.unk1E0->unk9 * 2 - c) * 12) >> 3;
            w->unk228 = w->base.unk1E0->unk8 * 12 + w->base.unk1D4 - (w->unk22C << 3);
        }
    }
    if (w->base.unk1E0->unk2 != 0xc0 && w->base.unk1E0->unk7 <= 0)
        return;
    if (w->unk20C != 0) {
        func_02067d58(&w->unk20C);
        if ((u32)w->base.unk8 < w->base.unk1E0->unk9 * 2 - 1) {
            w->unk22E = 2;
        } else {
            w->unk22E = 0;
        }
    }
    if (w->base.unk1E0->unk2 == 0xc0)
        return;
    if ((u32)w->base.unk8 < w->base.unk1E0->unk9 * 2 - 1) {
        w->base.unk1DA = 0;
    } else {
        w->base.unk1DA = 2;
    }
}
