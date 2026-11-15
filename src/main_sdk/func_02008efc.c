#include <nitro/types.h>

extern u32 data_0207d3dc;
extern u8 data_0207d3e0[];
void func_02006724(void *p); void func_02009654(void); void SND_AlarmInit(void);
void func_02008efc(void) { if (data_0207d3dc != 0) return; data_0207d3dc = 1; func_02006724(data_0207d3e0); func_02009654(); SND_AlarmInit(); }
