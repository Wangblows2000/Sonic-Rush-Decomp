#include <nitro/types.h>

typedef struct { u8 unk0[0xd8]; void (*fn)(void); } UnkStruct_02052d90;
void func_02052d44(void);
void func_02052c90(UnkStruct_02052d90 *s, void *a);
void func_02052d90(UnkStruct_02052d90 *s, void *a) { s->fn = func_02052d44; func_02052c90(s, a); }
