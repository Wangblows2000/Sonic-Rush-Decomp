#include <nitro/types.h>

typedef struct {
    u8 pad0[0x4C];
    u32 attr;
    u8 pad50[0x7C - 0x50];
} UnkSprite_02337048;

typedef struct {
    u8 pad0[0x30];
    u32 attr;
    u8 pad34[0x7C - 0x34];
} UnkSpriteE_02337048;

typedef struct {
    u8 pad0[0x34];
    u32 attr;
    u8 pad38[0x7C - 0x38];
} UnkSpriteF_02337048;

typedef struct {
    UnkSprite_02337048 a0;
    UnkSprite_02337048 a1;
    UnkSprite_02337048 a2[3];
    u8 pad26C[0x27C - 0x26C];
    UnkSprite_02337048 b0;
    UnkSprite_02337048 b1;
    UnkSprite_02337048 b2[2];
    UnkSprite_02337048 c[2];
    UnkSprite_02337048 d[2];
    u8 pad65C[0x688 - 0x65C];
    UnkSpriteE_02337048 e[5];
    u8 pad8F4[0x924 - 0x8F4];
    UnkSpriteF_02337048 f[3];
} UnkMgr_02337048;

typedef struct {
    u8 pad0[0x10];
    UnkMgr_02337048 *unk10;
} UnkTask_02337048;

extern UnkTask_02337048 *gHudTask;

#define SETPRIO_02337048(s, p) \
    do { \
        (s).attr &= ~0x60000; \
        (s).attr |= (p) << 17; \
    } while (0)

void Hud_SetOamPriority(u32 prio) {
    UnkMgr_02337048 *mgr;
    u32 i;
    UnkSpriteE_02337048 *e;
    UnkSpriteF_02337048 *f;

    if (gHudTask == NULL) {
        return;
    }
    mgr = gHudTask->unk10;
    SETPRIO_02337048(mgr->a0, prio);
    SETPRIO_02337048(mgr->a1, prio);
    for (i = 0; i < 3; i++) {
        SETPRIO_02337048(mgr->a2[i], prio);
    }
    SETPRIO_02337048(mgr->b0, prio);
    SETPRIO_02337048(mgr->b1, prio);
    for (i = 0; i < 2; i++) {
        SETPRIO_02337048(mgr->b2[i], prio);
    }
    for (i = 0; i < 2; i++) {
        SETPRIO_02337048(mgr->c[i], prio);
    }
    for (i = 0; i < 2; i++) {
        SETPRIO_02337048(mgr->d[i], prio);
    }
    e = mgr->e;
    f = mgr->f;
    SETPRIO_02337048(e[0], prio);
    SETPRIO_02337048(e[1], prio);
    SETPRIO_02337048(e[2], prio);
    SETPRIO_02337048(e[3], prio);
    SETPRIO_02337048(e[4], prio);
    SETPRIO_02337048(f[0], prio);
    SETPRIO_02337048(f[1], prio);
    SETPRIO_02337048(f[2], prio);
}
