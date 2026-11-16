#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0231b37c_P;

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_0231b37c_V;

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0xC - 6];
    s32 unkC;
    u8 pad10[0x20C - 0x10];
    UnkStruct_0231b37c_P pos[16];
    UnkStruct_0231b37c_V vel[16];
} UnkStruct_0231b37c;

s16 ObjSpd_DownSet(s16 a, s32 b);
s16 ObjSpd_UpSet(s16 a, s32 b, s32 c);

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

void func_ov000_0231b37c(UnkStruct_0231b37c *w) {
    u8 i;
    u8 j;
    s32 acc = 0x2a;
    s32 k;
    s32 v;

    w->unkC++;
    if (w->unk5 != 0)
        acc = -0x2a;
    for (i = 0; i < 8; i++) {
        j = 2;
        while (j-- != 0) {
            k = i + j * 8;
            w->pos[k].x += w->vel[k].x;
            w->pos[k].y += w->vel[k].y;
            v = w->pos[k].x;
            w->pos[k].x = CLAMP(v, -0xfff00, 0xfff00);
            v = w->pos[k].y;
            w->pos[k].y = CLAMP(v, -0xfff00, 0xfff00);
            w->vel[k].x = ObjSpd_DownSet(w->vel[k].x, 0x18);
            w->vel[k].y = ObjSpd_UpSet(w->vel[k].y, acc, 0x3000);
        }
    }
}
