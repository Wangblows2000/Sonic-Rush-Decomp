#include <nitro/types.h>

typedef struct { u32 timer; } UnkStruct_022cf0e4;
typedef struct { u8 pad[0x10]; UnkStruct_022cf0e4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
void func_020371c8(void *p);
void func_ov001_022cf4a0(s32 a);

#pragma thumb on
void func_ov001_022cf0e4(void) {
    UnkStruct_022cf0e4 *t = data_022b4574->p;
    t->timer++;
    if (t->timer >= 0x5a) {
        func_020371c8(data_022b4574);
        func_ov001_022cf4a0(0);
    }
}
#pragma thumb reset
