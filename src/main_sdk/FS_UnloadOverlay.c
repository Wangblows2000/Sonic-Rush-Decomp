#include <nitro/types.h>

typedef struct { u8 data[0x2c]; } FSOverlayInfo;
BOOL FS_LoadOverlayInfo(FSOverlayInfo *info, u32 target, u32 id);
BOOL FS_UnloadOverlayImage(FSOverlayInfo *info);
BOOL FS_UnloadOverlay(u32 target, u32 id) {
    FSOverlayInfo info;
    if (!FS_LoadOverlayInfo(&info, target, id) || !FS_UnloadOverlayImage(&info)) return 0;
    return 1;
}
