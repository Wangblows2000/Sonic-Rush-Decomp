/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void ExtraBossObj_State_HandBossSuctionWaitEnd();
extern u8 gExtraBossHandBossAnimTable[];
extern u8 data_ov001_0231f068[];
s32 Model_SetAnimation();
extern u8 data_ov001_0231f074[];

void ExtraBossObj_State_HandBossSuctionHold(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2) == 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x18) + 0x84);
    *(u32 *)((u8 *)p0 + 0x74) = (u32)ExtraBossObj_State_HandBossSuctionWaitEnd;
    Model_SetAnimation(t0, 0, *(u32 *)data_ov001_0231f068, *(u16 *)((u8 *)gExtraBossHandBossAnimTable + 0xb8), 0);
    Model_SetAnimation(t0, 4, *(u32 *)data_ov001_0231f074, *(u16 *)((u8 *)gExtraBossHandBossAnimTable + 0xba), 0);
    *(u8 *)((u8 *)t0 + 0x10c) = (*(u8 *)((u8 *)t0 + 0x10c) | 4);
    *(u8 *)((u8 *)t0 + 0x110) = (*(u8 *)((u8 *)t0 + 0x110) | 4);
    *(u32 *)((u8 *)t0 + 0x114) = 0x1000;
    Model_SetAnimation(t1, 0, *(u32 *)data_ov001_0231f068, *(u16 *)((u8 *)gExtraBossHandBossAnimTable + 0xbc), 0);
    Model_SetAnimation(t1, 4, *(u32 *)data_ov001_0231f074, *(u16 *)((u8 *)gExtraBossHandBossAnimTable + 0xbe), 0);
    *(u8 *)((u8 *)t1 + 0x10c) = (*(u8 *)((u8 *)t1 + 0x10c) | 4);
    *(u8 *)((u8 *)t1 + 0x110) = (*(u8 *)((u8 *)t1 + 0x110) | 4);
    *(u32 *)((u8 *)t1 + 0x114) = 0x1000;
}
