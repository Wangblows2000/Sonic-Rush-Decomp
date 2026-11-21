#include <nitro/types.h>

typedef struct UnkStruct_022f3c60_w {
    s32 type;
    u8 pad4[8];
    s32 unkC;
    u8 pad10[0x164 - 0x10];
    s32 unk164;
} UnkStruct_022f3c60_w;

typedef struct UnkStruct_022f3c60_o {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    u8 pad20[0xb8 - 0x20];
    u32 flagsB8;
    u8 padBC[0xf0 - 0xbc];
    u32 flagsF0;
    u8 padF4[0x1dc - 0xf4];
    UnkStruct_022f3c60_w *work;
} UnkStruct_022f3c60_o;

typedef struct UnkStruct_022f3c60_c {
    u8 pad0[0x308];
    s32 lives;
} UnkStruct_022f3c60_c;

extern u32 gRandSeed;
extern s32 data_ov001_02316910[];

UnkStruct_022f3c60_c *ExtraBoss_GetWork(void);
void ExtraBoss_SpawnDamageFx(s32 a, s32 x, s32 y, s32 b);
void ExtraBoss_CancelSwordFx(void);
void ExtraBoss_WeaponBoss_Action_Hurt(UnkStruct_022f3c60_o *p);
void ExtraBoss_WeaponBoss_Action_Defeat(UnkStruct_022f3c60_o *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Sound_PlayVoice(s32 a, s32 b, s32 c);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void ExtraBoss_WeaponBoss_OnHit(UnkStruct_022f3c60_o **self, UnkStruct_022f3c60_o **other) {
    UnkStruct_022f3c60_o *s = *self;
    UnkStruct_022f3c60_o *p = *other;
    UnkStruct_022f3c60_c *c = ExtraBoss_GetWork();
    UnkStruct_022f3c60_w *w = p->work;

    c->lives--;
    p->flagsB8 |= 0x200;
    p->flagsF0 |= 0x200;
    p->work->unkC = 0;
    ExtraBoss_SpawnDamageFx(0, s->x, s->y, 0);
    w->unk164 = 0;
    if (w->type == 5)
        ExtraBoss_CancelSwordFx();
    if (c->lives > 0) {
        ExtraBoss_WeaponBoss_Action_Hurt(p);
        Sound_PlaySeqArc(0, 0, 0x7b);
    } else {
        ExtraBoss_WeaponBoss_Action_Defeat(p);
        Sound_PlaySeqArc(0, 0, 0xba);
    }
    Sound_PlayVoice(0, 0, data_ov001_02316910[Rand() & 1]);
}
