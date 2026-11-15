#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0xec]; u16 unkEC[1]; } UnkStruct_0205fb30;
extern UnkStruct_0205fb30 *data_022c4d9c;
void func_0205fb30(s32 i) { UnkStruct_0205fb30 *p = data_022c4d9c; if (p != NULL) { while (!(*(vu16 *)0x4000004 & 2)) {} *(vu16 *)0x5000000 = p->unkEC[i]; } }

#pragma thumb reset
