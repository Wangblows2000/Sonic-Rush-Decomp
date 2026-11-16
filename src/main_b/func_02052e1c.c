#include <nitro/types.h>

typedef struct { u8 unk0[0xd8]; void (*fn)(void); u8 unkDC[0x1c]; s32 unkF8; } UnkStruct_02052e1c;
extern u32 gRandSeed;
void func_02052dd4(void);
void func_02052c90(UnkStruct_02052e1c *s, s32 a);
void func_02052e1c(UnkStruct_02052e1c *s) { s->fn = func_02052dd4; func_02052c90(s, 0); gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f; s->unkF8 = ((u16)(gRandSeed >> 16) & 0x3f) + 0x2d0; }
