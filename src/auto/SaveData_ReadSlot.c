/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gSaveState[];
s32 CARD_LockBackup();
s32 CARD_Unk_0200fa70();
s32 CARD_UnlockBackup();

s32 SaveData_ReadSlot(s32 p0, s32 p1) {
    s32 t0 = *(u16 *)gSaveState;
    CARD_LockBackup(*(u16 *)gSaveState, p1, gSaveState);
    s32 t1 = CARD_Unk_0200fa70((((s32)p0 * 0x320) + 0xc), p1, 0x314, 0, 0, 0);
    CARD_UnlockBackup(t0);
    return (t1 != 0);
}
