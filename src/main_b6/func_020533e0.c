#include <nitro/types.h>

typedef struct {
    s32 mode;
    s32 idx;
    u8 pad8[0x100];
    s32 unk108;
    s32 unk10c;
    s32 unk110;
    void *unk114;
    u32 unk118;
} UnkWork_020533e0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_020533e0 *work;
} UnkTask_020533e0;

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 zone;
} UnkState_020533e0;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
    u8 pade[0x56];
    u16 unk64;
} UnkProg_020533e0;

typedef s32 Tbl_020533e0[6];

extern UnkTask_020533e0 *gNaviMsgTask;
extern s32 data_0207b894;
extern UnkState_020533e0 data_022c4560;
extern UnkProg_020533e0 gSaveData;
extern Tbl_020533e0 data_0207567c[];
extern Tbl_020533e0 data_02075680[];
extern Tbl_020533e0 data_02075684[];
extern Tbl_020533e0 data_02075688[];
extern Tbl_020533e0 data_0207568c[];
extern Tbl_020533e0 data_02075690[];
extern u32 gRandSeed;
extern s32 data_0207566c[];
extern Tbl_020533e0 data_020756ac[];
extern Tbl_020533e0 data_020756b0[];
extern Tbl_020533e0 data_020756b4[];
extern Tbl_020533e0 data_020756b8[];
extern Tbl_020533e0 data_020756bc[];
extern Tbl_020533e0 data_020756c0[];

void func_020366f4(s32 a, s32 b, s32 c);
void func_02052af8(UnkWork_020533e0 *w, s32 a, s32 b);
void func_0201c070(void *p, s32 a);

void func_020533e0(s32 a, s32 b) {
    UnkWork_020533e0 *w;
    s32 snd = -1;

    if (gNaviMsgTask == NULL) {
        if (data_0207b894 != 1)
            return;
        if (a != 0x11)
            return;
        func_020366f4(0, 0, 0xcd);
        return;
    }
    w = gNaviMsgTask->work;
    if (w->mode == 0) {
        if (data_022c4560.unk0 == 0 && gSaveData.unkc < 3)
            return;
        if (data_022c4560.unk0 == 1 && (gSaveData.unk64 < 4 || (gSaveData.unk64 >= 0x1b && gSaveData.unk64 < 0x1c)))
            return;
    }
    func_02052af8(w, a, b);
    if (w->mode == 1) {
        switch (a) {
        case 0xf:
            w->unk108 = data_0207567c[w->idx][0];
            w->unk10c = 1;
            break;
        case 0x10:
            w->unk108 = data_02075680[w->idx][0];
            w->unk10c = 0x14;
            break;
        case 0x11:
            if (data_022c4560.zone == 6) {
                w->unk108 = data_02075684[w->idx][0];
            } else {
                gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
                if (((u16)(gRandSeed >> 16) & 1) == 0)
                    w->unk108 = 0xcd;
                else
                    w->unk108 = data_02075684[w->idx][0];
            }
            w->unk10c = 0x14;
            break;
        case 0x12:
            w->unk108 = data_02075688[w->idx][0];
            w->unk10c = 0x14;
            break;
        case 0x13:
            if (data_022c4560.zone != 7) {
                w->unk108 = data_0207568c[w->idx][0];
                w->unk10c = 1;
            } else {
                w->unk108 = data_02075690[w->idx][0];
                w->unk10c = 1;
            }
            break;
        }
        w->unk110 = 0;
        w->unk114 = NULL;
        return;
    }
    switch (a) {
    case 2:
        w->unk110 = data_0207566c[w->idx];
        w->unk108 = data_020756ac[w->idx][0];
        w->unk10c = 1;
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        snd = data_020756b0[w->idx][0];
        break;
    case 1:
        snd = data_020756b4[w->idx][0];
        break;
    case 4:
        snd = data_020756b8[w->idx][0];
        break;
    case 3:
        snd = data_020756bc[w->idx][0];
        break;
    case 6:
        snd = data_020756c0[w->idx][0];
        break;
    case 0:
    case 5:
    default:
        break;
    }
    w->unk114 = &w->unk118;
    if (snd == -1)
        return;
    func_0201c070(w->unk114, 0);
    func_020366f4((s32)w->unk114, data_0207566c[w->idx], snd);
}
