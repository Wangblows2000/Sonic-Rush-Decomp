#include <nitro/types.h>

void MATHi_CRC32UpdateRev(u32 a, u32 *t, u32 b, u32 c);
u32 MATH_CalcCRC32(u32 a, u32 b, u32 c) { u32 t = 0xffffffff; MATHi_CRC32UpdateRev(a, &t, b, c); return ~t; }
