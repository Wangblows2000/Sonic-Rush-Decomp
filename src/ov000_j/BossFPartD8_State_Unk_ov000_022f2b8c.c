#include <nitro/types.h>

typedef struct UnkStruct_022f2b8c {
    u8 pad0[0x68];
    void (*unk68)(struct UnkStruct_022f2b8c *p);
    u8 pad6C[0x434 - 0x6c];
    u16 unk434;
    u8 pad436[2];
    u16 unk438;
    u8 pad43A[0x458 - 0x43a];
    s32 unk458;
} UnkStruct_022f2b8c;

extern u16 data_ov000_02364120[][5];

void BossFPartD8_State_Unk_ov000_022f2b44(UnkStruct_022f2b8c *p);

void BossFPartD8_State_Unk_ov000_022f2b8c(UnkStruct_022f2b8c *p) {
    p->unk438 = data_ov000_02364120[p->unk458][p->unk434];
    p->unk68 = BossFPartD8_State_Unk_ov000_022f2b44;
    p->unk68(p);
}
