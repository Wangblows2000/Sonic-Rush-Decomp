#include <nitro/types.h>

s32 func_020030d0(void);
extern s32 data_0207d00c;
extern u16 data_02072570[];
extern s32 data_0207d008;
void func_02004f34(void) { s32 v = func_020030d0(); data_0207d00c = v; data_0207d008 = data_02072570[v >> 4] << 12; }
