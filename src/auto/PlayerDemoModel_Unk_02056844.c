/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02056118();
s32 PlayerDemoModel_Unk_02056428();
void PlayerDemoModel_Unk_02056840();
s32 PlayerDemoModel_Unk_02056908();

void PlayerDemoModel_Unk_02056844(u8 *p0) {
    func_02056118(p0, ((u8 *)p0 + 0x1334));
    PlayerDemoModel_Unk_02056428(p0);
    *(u8 *)((u8 *)p0 + 0x9) = (*(s8 *)((u8 *)p0 + 9) + 0x1);
    if (*(s8 *)((u8 *)p0 + 9) >= *(u8 *)((u8 *)p0 + 0xa)) {
        *(u32 *)p0 = (u32)PlayerDemoModel_Unk_02056840;
        return;
    }
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x10) | 0x40000000);
    *(u32 *)((u8 *)p0 + 0x10) = t0;
    PlayerDemoModel_Unk_02056908(p0, t0);
}

#pragma thumb reset
