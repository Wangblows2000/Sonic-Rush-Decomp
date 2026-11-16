#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_022e4ac4;

typedef struct {
    s32 x;
    s32 y;
    s32 unk8;
    s32 hidden;
} UnkPt_022e4ac4;

typedef struct {
    s32 x;
    s32 y;
} UnkCam_022e4ac4;

typedef struct {
    UnkPt_022e4ac4 pts[1];
    u8 pad10[0x9a8 - 0x10];
    UnkCam_022e4ac4 *cam;
    u8 pad9ac[0x10dc - 0x9ac];
    UnkSpr_022e4ac4 spr[8];
} UnkStruct_022e4ac4;

extern s32 data_ov001_023162f8[];

void AnimatorSprite_ProcessAnimation(UnkSpr_022e4ac4 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022e4ac4 *s);

#pragma thumb on
void ZoneMap_Unk_ov001_022e4ac4(UnkStruct_022e4ac4 *p) {
    s32 i;

    for (i = 0; i < 8; i++) {
        UnkPt_022e4ac4 *pt = &p->pts[data_ov001_023162f8[i]];
        UnkSpr_022e4ac4 *s = &p->spr[i];
        if (pt->hidden == 0) {
            s->x = pt->x - p->cam->x;
            s->y = pt->y - p->cam->y;
            s->x -= 0x1c;
            s->y += 7;
            if (s->x >= -0x38 && s->x < 0x100 && s->y >= -16 && s->y < 0xc0) {
                AnimatorSprite_ProcessAnimation(s, 0, 0);
                AnimatorSprite_Draw(s);
            }
        }
    }
}
#pragma thumb reset
