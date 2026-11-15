#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x34]; } UnkElem_02060470;
typedef struct { u8 unk0[0x1c]; UnkElem_02060470 arr[1]; } UnkStruct_02060470;
UnkElem_02060470 *func_02060470(UnkStruct_02060470 *s, s32 i) { return &s->arr[i]; }

#pragma thumb reset
