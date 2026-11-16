#include <nitro/types.h>

extern u16 gSaveState;
extern u8 gSaveSignature[];

void CARD_LockBackup(u16 id);
void CARD_UnlockBackup(u16 id);
s32 CARDi_ReadBackup(s32 a, void *buf, u32 size, s32 b, s32 c, s32 d);
s32 memcmp(const void *a, const void *b, u32 n);

BOOL SaveData_CheckSignature(void) {
    u16 id = gSaveState;
    u8 buf[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    CARD_LockBackup(id);
    if (CARDi_ReadBackup(0, buf, 12, 0, 0, 0) == 0) {
        CARD_UnlockBackup(id);
        return FALSE;
    }
    CARD_UnlockBackup(id);
    return memcmp(buf, gSaveSignature, 12) == 0;
}
