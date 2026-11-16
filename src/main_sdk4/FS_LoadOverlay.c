#include <nitro/types.h>

typedef struct {
    u8 data[0x2c];
} FSOverlayInfo;

BOOL FS_LoadOverlayInfo(FSOverlayInfo *info, u32 target, u32 id);
BOOL FS_LoadOverlayFile(FSOverlayInfo *info);
void FS_StartOverlay(FSOverlayInfo *info);

BOOL FS_LoadOverlay(u32 target, u32 id)
{
    FSOverlayInfo info;
    if (!FS_LoadOverlayInfo(&info, target, id) || !FS_LoadOverlayFile(&info)) {
        return FALSE;
    }
    FS_StartOverlay(&info);
    return TRUE;
}
