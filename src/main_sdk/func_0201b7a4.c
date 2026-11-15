#include <nitro/types.h>

typedef struct { u8 pad[0x2c]; u8 x2c; u8 pad2[0xf]; u8 x3c; } UnkStruct_0201b7a4;
void SND_SetPlayerVolume(u32 a, s32 v);
void func_02008ea8(u32 a);
void func_0201b698(UnkStruct_0201b7a4 *p);
void func_0201b7a4(UnkStruct_0201b7a4 *p) { if (p->x2c == 2) SND_SetPlayerVolume(p->x3c, -723); func_02008ea8(p->x3c); func_0201b698(p); }
