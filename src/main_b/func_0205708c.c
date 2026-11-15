#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; } UnkElem_0205708c;
typedef struct { u8 unk0[0x14]; UnkElem_0205708c arr[1]; } UnkStruct_0205708c;
UnkElem_0205708c *func_0205708c(UnkStruct_0205708c *s, s32 i) { return &s->arr[i]; }

#pragma thumb reset
