#include <nitro/types.h>

extern vu32 data_0207f09c;
u32 PMi_WaitBusy(void) { u32 v; do { v = data_0207f09c; } while (v != 0); return v; }
