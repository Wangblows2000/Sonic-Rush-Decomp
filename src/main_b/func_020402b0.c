#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_020402b0;
void func_020405bc(s32 v);
void func_020405e0(s32 a);
BOOL func_02040230(void);
void func_020402b0(UnkStruct_020402b0 *s) { if (s->unk2 != 0) { func_020405bc(s->unk2); func_020405e0(9); return; } if (func_02040230()) return; func_020405e0(9); }
