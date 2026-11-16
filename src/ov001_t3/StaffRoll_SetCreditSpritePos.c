#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    u32 flags;
} UnkSpr_022cfbc4;

s32 StaffRoll_Unk_ov001_022cfc5c(s32 n);
s32 StaffRoll_Unk_ov001_022cfc50(s32 n);
void StaffRoll_GetSpriteFrameSize(UnkSpr_022cfbc4 *s, s32 a, s32 *out);
void AnimatorSpriteDS_Draw(UnkSpr_022cfbc4 *s);

#pragma thumb on
void StaffRoll_SetCreditSpritePos(UnkSpr_022cfbc4 *s, s32 x, s32 n) {
    s32 y0;
    s32 y1;
    s32 h;

    y0 = StaffRoll_Unk_ov001_022cfc5c(n);
    y1 = StaffRoll_Unk_ov001_022cfc50(n);
    StaffRoll_GetSpriteFrameSize(s, 0, &h);
    if (-h <= y0 && y0 <= 0xc0) {
        s->flags &= ~1;
    } else {
        s->flags |= 1;
    }
    if (-h <= y1 && y1 <= 0xc0) {
        s->flags &= ~2;
    } else {
        s->flags |= 2;
    }
    s->x0 = x;
    s->y0 = y0;
    s->x1 = x;
    s->y1 = y1;
    AnimatorSpriteDS_Draw(s);
}
#pragma thumb reset
