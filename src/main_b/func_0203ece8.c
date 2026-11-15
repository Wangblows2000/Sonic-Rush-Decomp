#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203ece8;
void func_020405bc(s32 v);
void func_0203dc28(void);
BOOL func_0203ec9c(void);
void func_020405e0(s32 a);
void func_0203ece8(UnkStruct_0203ece8 *s) { if (s->unk2 != 0) { func_020405bc(s->unk2); func_0203dc28(); return; } if (func_0203ec9c()) return; func_020405e0(9); }
