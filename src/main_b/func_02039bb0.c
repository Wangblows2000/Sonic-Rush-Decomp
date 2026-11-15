#include <nitro/types.h>

typedef struct { u8 unk0[0x94]; u16 unk94; u8 unk96[0x802]; u16 unk898; } UnkStruct_02039bb0;
extern UnkStruct_02039bb0 *data_02072c78[];
extern u32 data_02072c70[];
u32 func_02006abc(s32 a);
void func_02039bb0(s32 i, u32 addr) { UnkStruct_02039bb0 *p = data_02072c78[i]; if (addr == func_02006abc(0)) return; ((u16 *)((u8 *)p + 0x98))[((addr - data_02072c70[i]) >> (p->unk94 + 5)) - p->unk898] = 0; }
