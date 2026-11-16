/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void ClearDemo_State_WaitParts();
s32 ClearDemoBg_Skip();
s32 ClearDemoName_Skip();
s32 ClearDemoActText_Skip();
s32 ClearDemoModel_Skip();
extern u8 gGameState[];
s32 ClearDemoTA_SkipRows();
s32 ClearDemoScore_SkipRows();
s32 ClearDemoBars_Skip();

void ClearDemo_SkipParts(s32 p0) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x344) | 0x500000);
    *(u32 *)((u8 *)p0 + 0x344) = t0;
    *(u32 *)((u8 *)p0 + 0x340) = (u32)ClearDemo_State_WaitParts;
    ClearDemoBg_Skip(p0, 0x340, (u32)ClearDemo_State_WaitParts, t0);
    ClearDemoName_Skip(p0);
    ClearDemoActText_Skip(p0);
    ClearDemoModel_Skip(p0);
    if (*(u32 *)((u8 *)gGameState + 0x10) == 2) {
        ClearDemoTA_SkipRows(p0);
    } else {
        ClearDemoScore_SkipRows(p0);
    }
    ClearDemoBars_Skip(p0);
}

#pragma thumb reset
