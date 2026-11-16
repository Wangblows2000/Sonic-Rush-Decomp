#include <nitro/types.h>

extern u8 gBossStatus[];
extern u32 gRandSeed;
void Boss4_SubState_SlamStopSpin();
s32 Boss4_Unk_ov000_022db900();
s32 Boss4_Unk_ov000_022dbc4c();
s32 Boss4_Unk_ov000_022dbb7c();
s32 Boss4_Unk_ov000_022dba50();
s32 Boss4_Unk_ov000_022dbab4();

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Boss4_Action_StartSlam(u8 *p0) {
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x18;
    *(u32 *)((u8 *)p0 + 0x220) = (u32)Boss4_SubState_SlamStopSpin;
    *(u32 *)((u8 *)p0 + 0x224) = 0;
    Boss4_Unk_ov000_022db900(p0, 0x60, 0, 0x18);
    *(u32 *)((u8 *)p0 + 0xbc4) = 0x80;
    *(u32 *)((u8 *)p0 + 0xbc8) = -0x600;
    *(u32 *)((u8 *)p0 + 0xbcc) = -0x600;
    *(u32 *)((u8 *)p0 + 0xbec) = Rand() & 1;
    if ((*(u32 *)((u8 *)p0 + 0x21c) & 2) == 0) {
        return;
    }
    Boss4_Unk_ov000_022dbc4c(0);
    Boss4_Unk_ov000_022dbb7c(0);
    Boss4_Unk_ov000_022dba50(0);
    Boss4_Unk_ov000_022dbab4(0);
}
