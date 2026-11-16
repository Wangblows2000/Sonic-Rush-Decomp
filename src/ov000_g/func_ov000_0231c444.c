#include <ov000_b/stageobj.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0231c444_P;

typedef struct {
    u8 pad0[0x3E8];
    void *unk3E8;
} UnkStruct_0231c444_O;

typedef struct {
    u8 unk0[0x8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x30 - 0x18];
    s16 unk30;
    u8 pad32[0x54 - 0x32];
    u32 unk54;
    u8 pad58[0x13E - 0x58];
    u16 unk13E;
    u8 pad140[0x14C - 0x140];
    void *unk14C;
    u8 pad150[0x1E0 - 0x150];
    UnkStruct_ObjParam *unk1E0;
    u8 pad1E4[0x204 - 0x1E4];
    UnkStruct_0231c444_O *unk204;
    u8 pad208[0x32C - 0x208];
    s16 unk32C;
    u8 pad32E[2];
    UnkStruct_0231c444_P pts[16];
    s16 cnt[16];
} UnkStruct_0231c444;

s16 ObjSpd_UpSet(s16 a, s32 b, s32 c);
s32 func_02066a9c(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);
void func_ov000_0232ab9c(void *w, s32 a, s32 b, s32 c);
void func_ov000_0231c21c(void *w);

void func_ov000_0231c444(UnkStruct_0231c444 *w) {
    s16 i;
    s32 flag;
    s32 t;

    flag = 1;
    if (w->unk204 != NULL && w->unk14C == NULL && w->unk204->unk3E8 != w) {
        w->unkC++;
        if (w->unkC > 0x18) {
            w->unk14C = w;
            w->unk13E &= ~0x1000;
            w->unkC = 0;
        }
    }
    w->unk30 = ObjSpd_UpSet(w->unk30, -6, 0x100);
    w->unk32C += 4;
    for (i = 0; i < 16; i++) {
        if (i == 3) {
            w->pts[i].x = w->unk10;
            w->pts[i].y = w->pts[i - 1].y;
        } else {
            if (w->pts[i].x == 0)
                break;
            if (i >= 4) {
                if (w->cnt[i] == 0) {
                    w->pts[i].y += w->unk32C;
                    w->pts[i].x += w->unk32C >> 1;
                    flag = 0;
                    if (func_02066a9c(w->pts[i].x >> 8, w->pts[i].y >> 8, 1, 2, 0, 0) < 0) {
                        w->cnt[i]++;
                    }
                } else {
                    w->cnt[i]++;
                    flag = 0;
                    if (w->cnt[i] > 0x60)
                        w->pts[i].x = 0;
                }
            } else if (i != 0) {
                w->pts[i].x = w->unk10;
                w->pts[i].y = w->pts[i - 1].y + 0xa00;
            } else {
                w->pts[i].x = w->unk10;
                w->pts[i].y = w->unk14 + 0x1800;
            }
        }
    }
    if (w->unk1E0->unk6 != 0) {
        t = w->unk1E0->unk6 * 10 - w->unk8;
        switch (t) {
        case 60:
            func_ov000_0232ab9c(w, 0, -0x18, 1);
            break;
        case 120:
            func_ov000_0232ab9c(w, 0, -0x18, 2);
            break;
        case 180:
            func_ov000_0232ab9c(w, 0, -0x18, 3);
            break;
        case 240:
            func_ov000_0232ab9c(w, 0, -0x18, 4);
            break;
        case 300:
            func_ov000_0232ab9c(w, 0, -0x18, 5);
            break;
        case 360:
            func_ov000_0232ab9c(w, 0, -0x18, 6);
            break;
        case 420:
            func_ov000_0232ab9c(w, 0, -0x18, 7);
            break;
        case 480:
            func_ov000_0232ab9c(w, 0, -0x18, 8);
            break;
        case 540:
            func_ov000_0232ab9c(w, 0, -0x18, 9);
            break;
        }
        if (t < 0)
            func_ov000_0231c21c(w);
    }
    w->unk8++;
    if (func_02066a9c(w->unk10 >> 8, w->unk14 >> 8, 1, 3, 0, 0) < 0)
        func_ov000_0231c21c(w);
    if (!(w->unk54 & 2))
        return;
    if (flag)
        w->unk54 |= 4;
}
