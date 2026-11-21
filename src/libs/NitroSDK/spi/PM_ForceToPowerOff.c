#include <nitro/types.h>

s32 PM_ForceToPowerOffAsync(void (*cb)(u32, u32 *), u32 *out);
void PMi_DummyCallback(u32 v, u32 *p);
u32 PMi_WaitBusy(void);
s32 PM_ForceToPowerOff(void) { u32 r; s32 res = PM_ForceToPowerOffAsync(PMi_DummyCallback, &r); if (res != 0) return res; PMi_WaitBusy(); return r; }
