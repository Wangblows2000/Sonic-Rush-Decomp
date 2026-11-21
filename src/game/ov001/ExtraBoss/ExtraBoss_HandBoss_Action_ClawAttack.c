#include <nitro/types.h>

extern u32 gRandSeed;
void ExtraBossObj_State_HandBossClawDelay(void);
void ExtraBossObj_State_None(void);
void ExtraBoss_StopAndFaceCenter(void *p);
static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void ExtraBoss_HandBoss_Action_ClawAttack(u8 *p) {
    u8 *q = *(u8 **)(*(u8 **)(p + 0x1dc) + 0x18);
    s32 r;
    *(void (**)(void))(p + 0x74) = ExtraBossObj_State_HandBossClawDelay;
    *(void (**)(void))(q + 0x74) = ExtraBossObj_State_None;
    *(u32 *)(p + 0xb8) &= ~0x200;
    *(u32 *)(p + 0xf0) &= ~0x200;
    *(u32 *)(p + 0x128) |= 0x200;
    *(u32 *)(p + 0x160) |= 0x200;
    *(u32 *)(q + 0xf0) |= 0x200;
    *(u32 *)(q + 0x128) |= 0x200;
    r = Rand() & 0xff;
    if (r < 0x4d) {
        *(s32 *)(p + 0x1c8) = 0x1e;
    } else if (r < 0xcd) {
        *(s32 *)(p + 0x1c8) = 0xb4;
    } else {
        *(s32 *)(p + 0x1c8) = 0x12c;
    }
    ExtraBoss_StopAndFaceCenter(p);
}
