#include <nitro/types.h>

typedef struct { u8 pad[8]; u32 count; u32 limit; s32 arg; } UnkStruct_022e3150;
typedef struct { u8 pad[0x10]; UnkStruct_022e3150 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
void func_ov001_022e3180(void *p, s32 a);

#pragma thumb on
void func_ov001_022e3150(void) {
    UnkStruct_022e3150 *t = data_022b4574->p;
    if (t->limit != 0) {
        t->count++;
        if (t->count >= t->limit) {
            func_ov001_022e3180(t, t->arg);
        }
    }
}
#pragma thumb reset
