#include <nitro/types.h>

typedef struct {
    u8 pad0[0x328];
    s16 unk328;
} UnkStruct_022f10dc_plr;

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x434 - 0x6c];
    s32 unk434;
    u16 unk438;
} UnkStruct_022f10dc;

extern UnkStruct_022f10dc_plr *gPlayers;

void func_ov000_022f10b0(UnkStruct_022f10dc *p);

void BossFPartD8_State_Unk_ov000_022f10dc(UnkStruct_022f10dc *p) {
    if ((u16)(s16)(gPlayers->unk328 - 0xf) <= 1)
        p->unk434 = 1;
    if (p->unk438 == 0)
        p->unk68 = func_ov000_022f10b0;
}
