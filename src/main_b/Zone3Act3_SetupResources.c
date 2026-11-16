#include <nitro/types.h>

extern void *gZoneActArchive;
extern u8 data_0207a070;
void func_02021310(void *a, void *b, void *c);
void func_020212e0(void *a);
void Zone3Act3_SetupResources(void) { u8 buf[0x5c]; func_02021310(buf, &data_0207a070, gZoneActArchive); func_020212e0(buf); }
