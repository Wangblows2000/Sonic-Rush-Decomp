#include <nitro/types.h>

typedef struct { u8 pad[0x508]; u32 x508; } UnkStruct_020187cc;
extern UnkStruct_020187cc *data_02080f18;
s32 func_02011b10(u32 a);
void func_020182e8(u32 a, s32 b);
s32 func_020187cc(void) { s32 r = func_02011b10(data_02080f18->x508); func_020182e8(1, r); if (r == 2) r = 0; return r; }
