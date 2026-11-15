#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203e388;
void func_020405bc(s32 v);
void func_020405e0(s32 a);
void func_0203e44c(void);
s32 func_0201154c(void (*cb)(void));
void func_0203e388(UnkStruct_0203e388 *s) { s32 r; if (s->unk2 != 0) { func_020405bc(s->unk2); func_020405e0(10); return; } r = func_0201154c(func_0203e44c); if (r != 0) { func_020405bc(r); func_020405e0(10); return; } func_020405e0(1); }
