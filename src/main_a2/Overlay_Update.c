#include <nitro/types.h>

extern u32 gOverlayLoadedId;
extern u32 gOverlayRequestedId;
extern u32 gOverlayIsLoaded;
extern void (*gOverlayCallback)(void);

BOOL FS_UnloadOverlay(u32 target, u32 id);
BOOL FS_LoadOverlay(u32 target, u32 id);

void Overlay_Update(void) {
    if (gOverlayLoadedId != gOverlayRequestedId) {
        if (gOverlayIsLoaded != 0) {
            FS_UnloadOverlay(0, gOverlayLoadedId);
            gOverlayIsLoaded = 0;
        }
        if (gOverlayIsLoaded == 0) {
            FS_LoadOverlay(0, gOverlayRequestedId);
            gOverlayLoadedId = gOverlayRequestedId;
            gOverlayIsLoaded = 1;
        }
    }
    if (gOverlayCallback != NULL) {
        gOverlayCallback();
        gOverlayCallback = NULL;
    }
}
