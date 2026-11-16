#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x434 - 0x6c];
    u16 unk434;
    u8 unk436[0x458 - 0x436];
    s32 unk458;
    u8 pad45C[0x57c - 0x45c];
    u8 unk57C;
} UnkStruct_022f2608;

extern u8 data_ov000_02363df0[];

void BossF_UpdateCameraShake(void *a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f2b8c(UnkStruct_022f2608 *p);
void BossFPartD8_State_Unk_ov000_022f25d0(UnkStruct_022f2608 *p);

void BossFPartD8_State_Unk_ov000_022f2608(UnkStruct_022f2608 *p) {
    BossF_UpdateCameraShake(p->unk436, 2, 6);
    if (!(p->unk57C & 2))
        return;
    p->unk434++;
    if (p->unk434 < data_ov000_02363df0[p->unk458])
        p->unk68 = BossFPartD8_State_Unk_ov000_022f2b8c;
    else
        p->unk68 = BossFPartD8_State_Unk_ov000_022f25d0;
}
