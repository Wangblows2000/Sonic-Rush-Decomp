/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBossStatus[];
void Boss4_SubState_Unk_ov000_022dd8c8();
s32 Boss4_Unk_ov000_022db900();
s32 Boss4_Unk_ov000_022dbc4c();
s32 Boss4_Unk_ov000_022dbb7c();
s32 Boss4_Unk_ov000_022dba50();
s32 Boss4_Unk_ov000_022dbab4();

void Boss4_Unk_ov000_022dd908(u8 *p0) {
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x19;
    *(u32 *)((u8 *)p0 + 0x220) = (u32)Boss4_SubState_Unk_ov000_022dd8c8;
    *(u32 *)((u8 *)p0 + 0x224) = 0;
    Boss4_Unk_ov000_022db900(p0, 0x60, 0, 0x19);
    *(u32 *)((u8 *)p0 + 0xbc4) = 0x20;
    *(u32 *)((u8 *)p0 + 0xbc8) = -0xc00;
    *(u32 *)((u8 *)p0 + 0xbcc) = -0xc00;
    if ((*(u32 *)((u8 *)p0 + 0x21c) & 2) == 0) {
        return;
    }
    Boss4_Unk_ov000_022dbc4c(0, 0x20);
    Boss4_Unk_ov000_022dbb7c(0);
    Boss4_Unk_ov000_022dba50(0);
    Boss4_Unk_ov000_022dbab4(0);
}
