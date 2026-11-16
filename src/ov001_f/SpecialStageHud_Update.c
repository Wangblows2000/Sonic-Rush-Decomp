#include <nitro/types.h>
typedef float f32;

typedef struct UnkSpr_02309e3c {
    u8 pad0[4];
    s16 x;
    s16 y;
    u16 anim;
    u8 padA[0x2c - 0xa];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSpr_02309e3c;

typedef struct UnkWork_02309e3c {
    u8 pad0[0x108];
    UnkSpr_02309e3c spr[25];
    UnkSpr_02309e3c sprA[8];
    UnkSpr_02309e3c sprB;
    UnkSpr_02309e3c sprC;
    s32 unkAE0;
    u8 padAE4[8];
    s32 unkAEC;
    s32 unkAF0;
    u8 padAF4[0xbb8 - 0xaf4];
    s32 unkBB8;
    u8 padBBC[0xbc4 - 0xbbc];
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
    s32 unkBD0;
    s32 unkBD4;
} UnkWork_02309e3c;

typedef struct UnkPlayer_02309e3c {
    u8 pad0[0x120];
    s32 unk120;
    u8 pad124[4];
    s32 unk128;
    u8 pad12C[0x14c - 0x12c];
    s16 unk14C;
    u8 pad14E[2];
    s16 unk150;
} UnkPlayer_02309e3c;

typedef struct UnkPos_02309e3c {
    u16 x;
    u16 y;
} UnkPos_02309e3c;

typedef struct UnkInfo_02309e3c {
    UnkPos_02309e3c pos[8];
    u8 pad20[2];
    u16 count;
} UnkInfo_02309e3c;

extern UnkWork_02309e3c *gSpecialStageHud;
extern u32 gRandSeed;
extern u16 data_ov001_023170b8[];

UnkPlayer_02309e3c *SpecialStage_GetPlayer(void);
s32 *SpecialStageMap_GetCurrentSegment(void);
void func_ov001_02312dd8(UnkPlayer_02309e3c *p, s32 a);
void SpecialStageHud_UpdateClear(void);
void func_ov001_02308b74(void);
s32 func_ov001_02314214(void);
UnkInfo_02309e3c *SpecialStageTrick_GetSequence(s32 a);
void AnimatorSprite_SetAnimation(UnkSpr_02309e3c *s, u16 anim);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void AnimatorSprite_ProcessAnimation(UnkSpr_02309e3c *s, s32 a, s32 b);
BOOL func_0202e5e4(f32 a, f32 b);
void func_ov001_02309228(void);
void SpecialStageHud_UpdateCheckpoint(void);
void SpecialStageHud_AnimateSprites(void);
void SpecialStageHud_UpdateGoal(void);
void SpecialStageHud_UpdateFail(void);
void SpecialStageHud_UpdateRingCounter(void);
void SpecialStageHud_UpdateQuotaCounter(void);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void SpecialStageHud_Update(void) {
    UnkWork_02309e3c *w = gSpecialStageHud;
    UnkPlayer_02309e3c *p;
    UnkInfo_02309e3c *info;
    s32 i;
    f32 f;

    p = SpecialStage_GetPlayer();
    w->unkAE0++;
    if (*SpecialStageMap_GetCurrentSegment() == 5 || *SpecialStageMap_GetCurrentSegment() == 6)
        func_ov001_02312dd8(p, 0);
    else if (w->unkBC4 < 0)
        func_ov001_02312dd8(p, 1);
    if (w->unkBD4 >= 0) {
        SpecialStageHud_UpdateClear();
        return;
    }
    if (w->unkAE0 >= 0x96)
        func_ov001_02308b74();
    if (w->unkAE0 >= 0x118) {
        s32 off[2] = {0, 0};
        w->unkBB8++;
        if (p->unk120 != 0) {
            off[0] = Rand() % 5 - 2;
            off[1] = Rand() % 5 - 2;
            w->unkBB8 = 0x20;
        }
        w->spr[9].x = off[0] + 0x18 + ((func_ov001_02314214() * 0xd0) >> 12);
        w->spr[9].y = off[1] + 0xac;
    }
    if (p->unk128 != 0) {
        info = SpecialStageTrick_GetSequence(p->unk150);
        if (p->unk14C == 0 && p->unk14C != w->unkAEC) {
            for (i = 0; i < info->count; i++)
                AnimatorSprite_SetAnimation(&w->sprA[i], data_ov001_023170b8[i]);
        }
        if (p->unk14C != w->unkAEC) {
            for (i = 0; i < info->count; i++) {
                if (i == p->unk14C) {
                    AnimatorSprite_SetAnimation(&w->sprA[i], data_ov001_023170b8[i] + 1);
                    w->sprA[i].flags |= 4;
                } else if (i == p->unk14C - 1) {
                    AnimatorSprite_SetAnimation(&w->sprA[i], 0x3e);
                    w->sprA[i].flags &= ~4;
                    Sound_PlaySeqArc(0, 0x10, 0);
                }
            }
            if (p->unk14C == 0x1234) {
                AnimatorSprite_SetAnimation(&w->sprA[info->count - 1], 0x3e);
                w->sprA[info->count - 1].flags &= ~4;
                Sound_PlaySeqArc(0, 0x10, 0);
            }
        }
        for (i = 0; i < info->count; i++) {
            w->sprA[i].x = info->pos[i].x;
            w->sprA[i].y = info->pos[i].y;
            AnimatorSprite_ProcessAnimation(&w->sprA[i], 0, 0);
        }
        AnimatorSprite_ProcessAnimation(&w->sprB, 0, 0);
        w->unkAEC = p->unk14C;
    } else {
        w->unkAEC = -1;
    }
    if (w->unkAF0 > 0) {
        w->unkAF0--;
        if (w->unkAF0 > 0x35) {
            f = (f32)(0x3c - w->unkAF0) / 7.0f;
            if (func_0202e5e4(f, 1.0f))
                f = 1.0f;
            f = 1.0f - f;
            w->spr[24].x = 148.0f + 256.0f * (f * (f * f));
            w->spr[24].y = 0x22;
        } else if (w->unkAF0 < 5) {
            f = (f32)w->unkAF0 / 5.0f;
            if (func_0202e5e4(f, 1.0f))
                f = 1.0f;
            f = 1.0f - f;
            w->spr[24].x = 138.0f - 256.0f * (f * (f * f));
            w->spr[24].y = 0x22;
        } else {
            if (w->unkAF0 & 1)
                w->spr[24].x--;
            w->spr[24].y = 0x22;
            if (w->unkAF0 == 0x33 && w->spr[24].anim != 0xb)
                AnimatorSprite_SetAnimation(&w->spr[24], w->spr[24].anim + 1);
        }
    }
    if (w->unkBC4 >= 0)
        func_ov001_02309228();
    if (w->unkBC8 >= 0) {
        SpecialStageHud_UpdateCheckpoint();
        SpecialStageHud_AnimateSprites();
        return;
    }
    if (w->unkBCC >= 0) {
        SpecialStageHud_UpdateGoal();
        SpecialStageHud_AnimateSprites();
        return;
    }
    if (w->unkBD0 >= 0)
        SpecialStageHud_UpdateFail();
    SpecialStageHud_UpdateRingCounter();
    SpecialStageHud_UpdateQuotaCounter();
    if (w->unkAE0 >= 0x17c && w->unkAE0 < 0x258) {
        w->sprC.x = 0x80;
        w->sprC.y = 0x6e;
        AnimatorSprite_ProcessAnimation(&w->sprC, 0, 0);
    }
    SpecialStageHud_AnimateSprites();
}
