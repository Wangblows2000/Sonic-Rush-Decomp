#include <nitro/types.h>

typedef struct { u8 unk0[0xd8]; void (*fn)(void); u8 unkDC[0x1c]; s32 unkF8; } UnkStruct_02052dd4;
void func_02052da8(void);
void func_02052c90(UnkStruct_02052dd4 *s, s32 a);
void func_02052dd4(UnkStruct_02052dd4 *s) { s->unkF8--; if (s->unkF8 > 0) return; s->fn = func_02052da8; func_02052c90(s, 1); }
