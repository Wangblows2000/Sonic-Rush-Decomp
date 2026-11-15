#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203e44c;
void func_020405e0(s32 a);
void OS_Terminate(void);
void func_0203e44c(UnkStruct_0203e44c *s) { if (s->unk2 != 8) return; func_020405e0(9); OS_Terminate(); }
