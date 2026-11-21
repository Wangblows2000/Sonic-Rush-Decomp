#include <nitro/types.h>

extern u8 gSEVolume;
s32 Sound_CalcDistanceVolume(s32 x0, s32 y0, s32 x1, s32 y1) { s32 dy = (y1 >> 8) - (y0 >> 8); s32 dx; s32 v; if (dy < 0) dy = -dy; dx = (x1 >> 8) - (x0 >> 8); if (dx < 0) dx = -dx; dx += dy; if (dx > 0x200) v = 0; else v = (0x200 - dx) >> 2; return (v * gSEVolume) >> 7; }
