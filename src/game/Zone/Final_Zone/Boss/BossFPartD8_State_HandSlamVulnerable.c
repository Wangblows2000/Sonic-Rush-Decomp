#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x434 - 0x6C];
    s32 unk434;
    u8 pad438[2];
    u8 unk43A[2];
    u16 unk43C;
    u16 unk43E;
} UnkStruct_022f3758;

void BossF_UpdateCameraShake(void *p, s32 a, s32 b);
extern u32 gGameState;
void BossFPartD8_LoadModelPalettesB(u32 a, s32 b);
void BossFPartD8_LoadModelPalettesA(u32 a, s32 b);
void BossFPartD8_DisableHitboxes(UnkStruct_022f3758 *s);
void BossFPartD8_State_HandSlamLift(void);

void BossFPartD8_State_HandSlamVulnerable(UnkStruct_022f3758 *s) {
    BossF_UpdateCameraShake(s->unk43A, 2, 6);
    s->unk43E++;
    if ((s->unk43E & 3) == 0) {
        if (s->unk43E & 0x8000) {
            if (s->unk434 != 0) {
                BossFPartD8_LoadModelPalettesB(gGameState, 0);
            } else {
                BossFPartD8_LoadModelPalettesA(gGameState, 0);
            }
        } else {
            if (s->unk434 != 0) {
                BossFPartD8_LoadModelPalettesB(gGameState, 1);
            } else {
                BossFPartD8_LoadModelPalettesA(gGameState, 1);
            }
        }
        s->unk43E &= ~3;
        s->unk43E ^= 0x8000;
    }
    if (s->unk43C != 0) {
        s->unk43C--;
    }
    if (s->unk43C != 0) {
        return;
    }
    if (s->unk43E & 0x8000) {
        return;
    }
    BossFPartD8_DisableHitboxes(s);
    s->unk68 = BossFPartD8_State_HandSlamLift;
}
