#include <nitro/types.h>

extern u8 data_022c4eec[2];
void ObjDraw_SetGlobalOffset(u8 a, u8 b) { data_022c4eec[0] = a; data_022c4eec[1] = b; }
