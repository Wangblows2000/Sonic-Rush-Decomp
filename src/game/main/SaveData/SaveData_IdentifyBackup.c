#include <nitro/types.h>

extern u16 gSaveState;
void CARD_LockBackup(u16 id);
s32 CARD_IdentifyBackup(u32 a);
void CARD_UnlockBackup(u16 id);
s32 SaveData_IdentifyBackup(void) { u16 id = gSaveState; s32 r; CARD_LockBackup(id); r = CARD_IdentifyBackup(0xd01); CARD_UnlockBackup(id); return r; }
