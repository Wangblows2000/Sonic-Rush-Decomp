#include <nitro/types.h>

typedef struct { u8 pad[0x74]; s32 a; u8 pad2[8]; s32 b; } UnkStruct_022e5a48_b;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022e5a48_b *p; } UnkStruct_022e5a48;
extern UnkStruct_022e5a48 *data_022b4574;
void func_ov001_022e6e9c(void);

#pragma thumb on
void func_ov001_022e5a48(s32 v) {
    UnkStruct_022e5a48_b *t = data_022b4574->p;
    t->a = 0;
    t->b = v;
    data_022b4574->fn = func_ov001_022e6e9c;
}
#pragma thumb reset
