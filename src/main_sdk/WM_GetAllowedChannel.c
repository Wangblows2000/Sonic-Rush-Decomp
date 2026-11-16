#include <nitro/types.h>

u32 func_02010b54(void);
u16 WM_GetAllowedChannel(void) { if (func_02010b54() != 0) return 0x8000; return *(u16 *)0x27ffcfa; }
