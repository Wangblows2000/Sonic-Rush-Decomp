#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231d4c0[];
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_AdvanceSwordFxStep(void *p);
void ExtraBossObj_State_WeaponBossSwordFollowThrough(void *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Sound_PlayVoice(s32 a, s32 b, s32 c);

void ExtraBossObj_State_WeaponBossSwordSlash(u8 *p) {
    u8 *r = *(u8 **)(p + 0x84);
    if (!(r[0x10c] & 2)) {
        return;
    }
    *(void (**)(void *))(p + 0x74) = ExtraBossObj_State_WeaponBossSwordFollowThrough;
    Model_SetAnimation(r, 0, data_ov001_0231f068, data_ov001_0231d4c0[*(s32 *)(p + 0x1cc)] + 2, 0);
    if (*(s32 *)(p + 0x1cc) == 1) {
        *(s32 *)(p + 0x50) = -0x6000;
    }
    ExtraBoss_AdvanceSwordFxStep(*(void **)(*(u8 **)(p + 0x1dc) + 0x10));
    ExtraBossObj_State_WeaponBossSwordFollowThrough(p);
    Sound_PlaySeqArc(0, 0, 0xad);
    Sound_PlayVoice(0, 0, 0xd0);
}
