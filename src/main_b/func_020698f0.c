#include <nitro/types.h>

typedef struct { u8 unk0[0x94]; u8 unk94[0x48]; u8 unkDC[1]; } UnkStruct_020698f0;
void func_020698bc(void *a, void *b, s32 c);
void func_020698f0(UnkStruct_020698f0 *s, s32 c) { func_020698bc(s->unk94, s->unkDC, c); }
