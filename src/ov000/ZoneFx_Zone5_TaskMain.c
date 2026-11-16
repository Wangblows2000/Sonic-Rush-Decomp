#include <nitro/types.h>

extern void *gZoneFxWork;

void ZoneFx_Zone5_UpdatePalettes(void *p);

void ZoneFx_Zone5_TaskMain(void) {
    ZoneFx_Zone5_UpdatePalettes(gZoneFxWork);
}
