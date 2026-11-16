#include <ov000_c/stageobj.h>

typedef struct {
    u16 b0 : 5;
    u16 b5 : 11;
} UnkStruct_Bits5;

typedef struct {
    u8 pad0[0x24];
    UnkStruct_Bits5 unk24;
} UnkStruct_0208922c;

typedef struct {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    s32 unk210;
    s16 unk214;
    s16 unk216;
    s16 unk218;
    u16 unk21A;
    s16 unk21C;
    u8 pad21E[2];
    s32 unk220;
    u8 pad224[0x254 - 0x224];
    s32 unk254;
} UnkStruct_022f7180;

extern volatile UnkStruct_0208922c gDisplayMain;
extern UnkStruct_Bits5 data_02089250;
extern UnkStruct_022c4560 data_022c4560;
extern u16 data_ov000_02365c54[];
extern UnkStruct_Player *gPlayers;

void func_ov000_022f7804(UnkStruct_022f7180 *w);
s32 MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
BOOL func_020254d4(UnkStruct_Vec *v, s32 *a, s32 *b);

#define FX_MUL(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))

void BossFPartDC_State_Unk_ov000_022f7180(UnkStruct_022f7180 *w) {
    BOOL b;
    s32 i;
    s32 d;
    s16 v;
    s32 x;
    u16 n;
    u16 cur;
    UnkStruct_Vec vec;
    s32 ang;
    s32 tmp;
    s32 k;
    u16 r;

    func_ov000_022f7804(w);
    if (w->unk21A-- == 0) {
        b = TRUE;
        if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
            b = FALSE;
        if (b)
            i = 1;
        else
            i = data_022c4560.unk14;
        r = Rand();
        w->unk21A = (r & 0xf) + data_ov000_02365c54[i];
        if (gPlayers->unk10 < w->unk220)
            w->unk20C |= 4;
        else
            w->unk20C &= ~4;
    }
    k = w->unk218;
    d = w->unk220 - gPlayers->unk10;
    if (d < 0)
        d = -d;
    v = k + (k * d) / 0x8000;
    if (w->unk20C & 4)
        w->base.unk2E -= v;
    else
        w->base.unk2E += v;
    x = w->base.unk2E;
    if (x < -0x800)
        x = -0x800;
    else if (x > 0x800)
        x = 0x800;
    w->base.unk2E = x;
    w->unk220 += w->base.unk2E;
    x = w->unk220;
    if (x < 0x1800)
        x = 0x1800;
    else if (x > 0x32800)
        x = 0x32800;
    w->unk220 = x;
    if (w->base.unkC != 0) {
        w->base.unkC--;
        if (w->base.unkC == 0)
            w->unk214 = w->base.unk2E;
    }
    if (w->base.unkC == 0) {
        w->unk20C |= 8;
        w->unk216 += 0x88;
        if (w->unk216 >= 0x1000) {
            w->unk216 = 0x1000;
            if (gDisplayMain.unk24.b0 == 0)
                w->base.unk54 |= 4;
        }
        w->base.unk2E = w->unk214 + FX_MUL(-w->unk214, w->unk216);
        w->unk254 = FX_MUL(w->unk216, -0x3333) + 0x34cc;
        if (gDisplayMain.unk24.b0 != 0 && !(w->unk210 & 1))
            data_02089250.b0 -= 1;
    } else if (!(w->unk210 & 1)) {
        vec.x = w->unk220 << 4;
        vec.y = -0x300000;
        vec.z = 0;
        if (func_020254d4(&vec, &ang, &tmp) == 0) {
            ang = 0x80 - ang;
            ang = 0x40 - (ang < 0 ? -ang : ang);
            if (ang < 0)
                x = 0;
            else if (ang > 0x40)
                x = 0x40;
            else
                x = ang;
            n = (u32)x >> 2;
            n = n < 2 ? 2 : (n > 7 ? 7 : n);
        } else {
            n = 2;
        }
        cur = gDisplayMain.unk24.b0;
        if (cur < n)
            data_02089250.b0 += 1;
        else if (cur > n)
            data_02089250.b0 -= 1;
    }
    x = w->base.unk2E;
    if (x < 0)
        x = -x;
    w->unk21C += x;
    if (w->unk21C >= 0x2000) {
        w->unk21C -= 0x2000;
        MapObject_Spawn(0xdd, w->unk220, 0x30000, 0xf, 0, 0, 0, 0, 0);
    }
    w->unk210++;
}
