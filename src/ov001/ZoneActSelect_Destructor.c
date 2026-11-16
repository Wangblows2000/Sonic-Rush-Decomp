#include <nitro/types.h>

extern void *gZoneActSelectWork;
void func_ov001_022e14b8(void *p);
void func_ov001_022e1444(void *p);
void func_ov001_022e1400(void);

#pragma thumb on
void ZoneActSelect_Destructor(void) {
    func_ov001_022e14b8(gZoneActSelectWork);
    func_ov001_022e1444(gZoneActSelectWork);
    func_ov001_022e1400();
    gZoneActSelectWork = NULL;
}
#pragma thumb reset
