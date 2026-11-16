/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov000_0235e124();
s32 func_ov000_0235e068();
s32 func_ov000_0235e018();
s32 func_ov000_0235dfc4();
s32 func_ov000_0235def0();
s32 func_ov000_0235ddac();
s32 ResultsScreen_HasPartnerDecided();
s32 ResultsScreen_HasPartnerQuit();
s32 func_ov000_0235cfc4();
void func_ov000_0235e8ac();
void ResultsScreen_State_FadeOutExit();

void ResultsScreen_State_WaitPartner(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov000_0235e124(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    func_ov000_0235e068(t0);
    func_ov000_0235e018(t0);
    func_ov000_0235dfc4(t0);
    func_ov000_0235def0(t0, *(u32 *)((u8 *)t0 + 0x20), 0x1000);
    func_ov000_0235ddac(t0);
    s32 t1 = ResultsScreen_HasPartnerDecided(t0);
    s32 t2 = ResultsScreen_HasPartnerQuit(t0);
    s32 t3 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t3;
    if (t1 == 0) {
        *(u32 *)t0 = 0;
        func_ov000_0235cfc4(t0, t3);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov000_0235e8ac;
        return;
    }
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0xc) = 1;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ResultsScreen_State_FadeOutExit;
    }
}

#pragma thumb reset
