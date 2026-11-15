#include <nitro/types.h>

typedef struct { s32 timer; } UnkStruct_022cf0b4;
typedef struct { u8 pad[0x10]; UnkStruct_022cf0b4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
void func_020521b4(void);
void func_020371c8(void *p);
void func_02054e4c(void);

#pragma thumb on
void func_ov001_022cf0b4(void) {
    UnkStruct_022cf0b4 *t = data_022b4574->p;
    t->timer++;
    if (t->timer >= 0xb4) {
        func_020521b4();
        func_020371c8(data_022b4574);
        func_02054e4c();
    }
}
#pragma thumb reset
