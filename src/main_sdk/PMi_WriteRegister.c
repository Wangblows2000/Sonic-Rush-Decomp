#include <nitro/types.h>

s32 PMi_WriteRegisterAsync(u32 a, u32 b, void (*cb)(u32, u32 *), u32 *out);
void PMi_DummyCallback(u32 v, u32 *p);
u32 PMi_WaitBusy(void);
s32 PMi_WriteRegister(u32 a, u32 b) { u32 r; s32 res = PMi_WriteRegisterAsync(a, b, PMi_DummyCallback, &r); if (res != 0) return res; PMi_WaitBusy(); return r; }
