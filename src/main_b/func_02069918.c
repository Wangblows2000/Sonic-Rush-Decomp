#include <nitro/types.h>

typedef struct { u8 unk0[0x94]; u8 unk94[0x48]; u8 unkDC[1]; } UnkStruct_02069918;
void func_0206990c(void *a, void *b, s16 c);
void func_02069918(UnkStruct_02069918 *s, s16 c) { func_0206990c(s->unk94, s->unkDC, c); }
