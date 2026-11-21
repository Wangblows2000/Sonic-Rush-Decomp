#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x218 - 0x20C];
    s32 unk218;
    u8 pad21C[0x35C - 0x21C];
    s32 unk35C;
    u8 pad360[0x4C6 - 0x360];
    s16 unk4C6;
} UnkStruct_022f68c4;

void BossFPartDE_ShiftEndsTowardX(UnkStruct_022f68c4 *w, s32 maxv, s32 maxd) {
    s32 half = (w->unk35C - w->unk218) >> 1;
    s32 mid = w->unk218 + half;
    s32 d = w->base.unk10 - mid;
    s32 p;

    d >>= 4;
    d = d < -maxd ? -maxd : (d > maxd ? maxd : d);
    w->unk4C6 += (s16)d;
    w->unk4C6 = w->unk4C6 < -maxv ? -maxv : (w->unk4C6 > maxv ? maxv : w->unk4C6);
    p = mid + w->unk4C6;
    p = p < half ? half : (p > 0x34000 - half ? 0x34000 - half : p);
    w->unk218 = p - half;
    w->unk35C = p + half;
}
