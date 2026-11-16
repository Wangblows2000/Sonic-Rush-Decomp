#include <nitro/types.h>

void ExtraBossPlayer_UpdateInvincibility(void *p);
void ExtraBossPlayer_ChangeState(void *p, s32 a);
void ExtraBossPlayer_SpawnBlzShotFx(void *p, s32 x, s32 y, s32 z);
void ExtraBossPlayer_SpawnSpecialShot(void *p, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void ExtraBossPlayer_ApplyMoveInputHalf(void *p);

void ExtraBossPlayer_State_SpecialCharge(u8 *p) {
    ExtraBossPlayer_UpdateInvincibility(p);
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) <= 0) {
        ExtraBossPlayer_ChangeState(p, 0xa);
        ExtraBossPlayer_SpawnBlzShotFx(p, *(s32 *)(p + 0x18), *(s32 *)(p + 0x1c), 0);
        ExtraBossPlayer_SpawnSpecialShot(p, *(s32 *)(p + 0x18), *(s32 *)(p + 0x1c), 0);
        Sound_PlaySeqArc(0, 0, 0x12);
    }
    ExtraBossPlayer_ApplyMoveInputHalf(p);
}
