#include <nitro/types.h>

extern void *gZoneActSelectWork;
void ZoneActSelect_ReleaseSprites(void *p);
void ZoneActSelect_ReleaseAssets(void *p);
void ZoneActSelect_ResetDisplay(void);

#pragma thumb on
void ZoneActSelect_Destructor(void) {
    ZoneActSelect_ReleaseSprites(gZoneActSelectWork);
    ZoneActSelect_ReleaseAssets(gZoneActSelectWork);
    ZoneActSelect_ResetDisplay();
    gZoneActSelectWork = NULL;
}
#pragma thumb reset
