#include <nitro/types.h>

typedef struct UnkStruct_022f3d48 {
    u8 pad0[0x68];
    void (*unk68)(struct UnkStruct_022f3d48 *p);
    u8 pad6C[0x438 - 0x6c];
    u16 unk438;
    u8 pad43A[2];
    u16 unk43C;
    u8 pad43E[0x458 - 0x43e];
    s32 unk458;
} UnkStruct_022f3d48;

extern u16 data_ov000_02364248[][6];

void BossFPartD8_State_HandSlamTrack(UnkStruct_022f3d48 *p);
void BossFPartD8_State_HandSlamStrike(UnkStruct_022f3d48 *p);

void BossFPartD8_State_HandSlamNext(UnkStruct_022f3d48 *p) {
    p->unk43C = 0;
    if (data_ov000_02364248[p->unk458][p->unk438] != 0)
        p->unk68 = BossFPartD8_State_HandSlamTrack;
    else
        p->unk68 = BossFPartD8_State_HandSlamStrike;
    p->unk68(p);
}
