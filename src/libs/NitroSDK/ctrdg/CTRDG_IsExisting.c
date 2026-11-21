#include <nitro/types.h>
#include <main_sdk2/ctrdg.h>

BOOL CTRDG_IsExisting(void)
{
    BOOL ret = TRUE;
    UnkCtrdgLock lock;
    UnkCtrdgRomCycle cycle;
    u8 fixed;
    UnkCtrdgModuleInfo *info = CTRDG_MODULE_INFO;
    UnkCtrdgHeader *hdr = CTRDG_HEADER;

    if (info->moduleID == 0xffff) {
        return FALSE;
    }
    if (info->detectPullOut == TRUE) {
        return FALSE;
    }
    CTRDGi_LockByProcessor(data_02080f54.lockId, &lock);
    CTRDGi_ChangeLatestAccessCycle(&cycle);
    fixed = hdr->fixed;
    if ((fixed == 0x96 && info->moduleID != hdr->moduleID) ||
        (fixed != 0x96 && info->moduleID != *(u16 *)0x0801fffe) ||
        (info->gameCode != hdr->gameCode && info->isAgbCartridge)) {
        ret = FALSE;
        info->detectPullOut = TRUE;
    }
    CTRDGi_RestoreAccessCycle(&cycle);
    CTRDGi_UnlockByProcessor(data_02080f54.lockId, &lock);
    return ret;
}
