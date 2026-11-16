#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x44 - 0x30];
    u16 pal;
    u8 pad46[2];
} UnkSprite_022e0ad0;

typedef struct {
    s32 v[3];
} UnkScore_022e0ad0;

typedef struct {
    u8 pad0[8];
    u32 timer;
    u8 padc[0x14 - 0xc];
    UnkScore_022e0ad0 score[8];
    u8 pad74[0xd8 - 0x74];
    s32 unkd8;
    u8 paddc[0x360 - 0xdc];
    UnkSprite_022e0ad0 spr[37];
    u8 padx[0xe10 - 0x360 - 37 * 0x48];
    u32 anim[37];
} UnkStruct_022e0ad0;

extern s32 data_ov001_0231c3ec;
extern s32 gZoneActSelectResult;

void AnimatorSprite_SetAnimation(UnkSprite_022e0ad0 *s, u16 id);
void AnimatorSprite_ProcessAnimation(UnkSprite_022e0ad0 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSprite_022e0ad0 *s);

#pragma thumb on
void ZoneActSelect_Unk_ov001_022e0ad0(UnkStruct_022e0ad0 *p) {
    s32 v;
    s32 i;
    u16 pal;
    s16 ys;
    BOOL flip;
    s32 k;
    s32 n;
    s32 *pd;
    UnkSprite_022e0ad0 *s;
    s32 digit[7];
    u32 id;

    for (i = 0; i < 3; i++) {
        s32 j;
        v = p->score[data_ov001_0231c3ec].v[i];
        k = 1;
        for (j = 0; j < 7; j++) {
            digit[j] = v % (k * 10) / k;
            k *= 10;
        }
        for (j = 6; j >= 0; j--) {
            if (digit[j] == 0) digit[j] = -1;
            if (digit[j] > 0) break;
        }
        if (digit[0] < 0) digit[0] = 0;
        ys = i * 0x20 + 0x4e;
        j = 0;
        pd = digit;
        for (; j < 7; j++) {
            id = *pd;
            if ((s32)id < 0) break;
            n = i * 7 + 0x10 + j;
            s = &p->spr[n];
            id *= 3;
            if (i == gZoneActSelectResult) {
                if (p->unkd8 != 0 && p->timer < 0x3c) {
                    id += 2;
                    pal = 9;
                    flip = TRUE;
                } else {
                    id += 1;
                    pal = 8;
                    flip = FALSE;
                }
            } else {
                pal = 7;
                flip = FALSE;
            }
            if (id != p->anim[n]) {
                AnimatorSprite_SetAnimation(s, id);
                p->anim[n] = id;
            }
            s->pal = pal;
            if (flip) {
                s->flags |= 4;
            } else {
                s->flags &= ~4;
            }
            s->x = j * -8 + 0xa0;
            s->y = ys;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
            pd++;
        }
    }
}
#pragma thumb reset
