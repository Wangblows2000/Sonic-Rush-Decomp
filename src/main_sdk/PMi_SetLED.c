#include <nitro/types.h>

s32 PMi_SetLEDAsync(u32 a, void (*cb)(u32, u32 *), u32 *out);
void PMi_DummyCallback(u32 v, u32 *p);
u32 PMi_WaitBusy(void);
s32 PMi_SetLED(u32 a) { u32 r; s32 res = PMi_SetLEDAsync(a, PMi_DummyCallback, &r); if (res != 0) return res; PMi_WaitBusy(); return r; }
