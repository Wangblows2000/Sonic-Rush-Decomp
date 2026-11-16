#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s32 unk434;
    u16 unk438;
    u8 unk43A[2];
    u16 unk43C;
    u8 pad43E[0x458 - 0x43E];
    s32 unk458;
} UnkStruct_022f3c84;

extern u16 data_ov000_02364248[][6];

void BossF_UpdateCameraShake(void *p, s32 a, s32 b);
void BossFPartD8_Unk_ov000_022f49f4(UnkStruct_022f3c84 *s);
void BossFPartD8_Unk_ov000_022f499c(UnkStruct_022f3c84 *s, s32 a);
void BossFPartD8_State_Unk_ov000_022f3bb8(void);

void BossFPartD8_State_Unk_ov000_022f3c84(UnkStruct_022f3c84 *s) {
    u16 t;

    BossF_UpdateCameraShake(s->unk43A, 2, 6);
    t = data_ov000_02364248[s->unk458][s->unk438];
    if (s->unk458 >= 6 && s->unk458 <= 8)
        BossFPartD8_Unk_ov000_022f49f4(s);
    else
        BossFPartD8_Unk_ov000_022f499c(s, s->unk434);
    if (t < s->unk43C++ || (t > 0x2d && s->base.unk2E == 0))
        s->base.unk68 = BossFPartD8_State_Unk_ov000_022f3bb8;
}
