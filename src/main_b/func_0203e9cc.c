#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203e9cc;
void func_020405e0(s32 a);
void func_0203e9cc(UnkStruct_0203e9cc *s) { if (s->unk2 != 0) { func_020405e0(10); return; } func_020405e0(0); }
