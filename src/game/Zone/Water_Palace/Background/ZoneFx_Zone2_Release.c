#include <nitro/types.h>

void ZoneFx_Zone2_Shutdown(void);

void ZoneFx_Zone2_Release(void) {
    ZoneFx_Zone2_Shutdown();
}
