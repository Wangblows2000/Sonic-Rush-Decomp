// cc: 1.2/sp2p3
#include <nitro/types.h>

u32 func_02010b54(void);
u16 func_020111e4(void) { if (func_02010b54() != 0) return 0x8000; return *(u16 *)0x27ffcfa; }
