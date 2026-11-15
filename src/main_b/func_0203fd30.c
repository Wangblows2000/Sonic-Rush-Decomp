#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203fd30;
void func_020405bc(s32 v);
void func_020405e0(s32 a);
void func_0203fd30(UnkStruct_0203fd30 *s) { if (s->unk2 == 0) return; func_020405bc(s->unk2); func_020405e0(9); }
