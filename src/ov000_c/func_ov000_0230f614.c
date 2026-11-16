#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

typedef struct {
    u32 unk0;
    s32 unk4;
    u8 pad8[0x64 - 8];
} UnkStruct_Plr;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Plr data_ov000_0236aad8[];
extern s32 data_ov000_0236a060;

s32 func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_0230f590(void);

void func_ov000_0230f614(UnkStruct_StageObj *w) {
    u32 n;
    s32 x;
    s32 d;
    s32 per;
    s32 sum;
    s16 v;
    UnkStruct_ObjParam *p;
    u32 i;

    w->unkC = (w->unk1E0->unk6 & 0x3f) * (w->unk1E0->unk8 + 1) + data_ov000_0236a060;
    w->unk42 >>= 3;
    w->unk42 += w->unk1E0->unk7 << 6;
    x = w->unk10;
    if ((x < data_ov000_0236aad8[0].unk4 - 0x1000 || x > data_ov000_0236aad8[0].unk4 - 0x11000) &&
        (x < data_ov000_0236aad8[1].unk4 - 0x1000 || x > data_ov000_0236aad8[1].unk4 - 0x11000)) {
        sum = 0;
        v = 0;
        d = data_022c4560.unk20 - w->unkC;
        if (d > 0) {
            per = (w->unk1E0->unk8 + 1) << 6;
            w->unkC += per * ((u32)d / per);
        }
        p = w->unk1E0;
        n = (u32)(data_022c4560.unk20 - w->unkC) % ((p->unk8 + 1) << 6);
        for (i = 0; i < n; i++) {
            v += w->unk40;
            if (v > w->unk42)
                v = w->unk42;
            sum += v;
        }
        func_ov000_022fad3c(0xef, w->unk10, w->unk14 + sum, 0, 0, p->unk7, 0, 0, 0);
    }
    w->unk1B8 = NULL;
    w->unk5C |= 0x20;
    w->unk68 = func_ov000_0230f590;
}
