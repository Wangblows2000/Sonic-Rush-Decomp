#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203ebc8;
extern s32 data_022c196c;
void func_020405bc(s32 v);
void func_020405e0(s32 a);
void func_0204045c(UnkStruct_0203ebc8 *s);
void func_0203ebc8(UnkStruct_0203ebc8 *s) { if (s->unk2 != 0) { func_020405e0(9); func_020405bc(s->unk2); return; } if ((u32)(data_022c196c - 6) <= 1) func_0204045c(s); func_020405e0(1); }
