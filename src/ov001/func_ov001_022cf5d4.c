#include <nitro/types.h>

typedef struct { s32 timer; } UnkStruct_022cf5d4;
typedef struct { u8 pad[0x10]; UnkStruct_022cf5d4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
void func_02037134(void);
void func_ov001_022d6d4c(void);

#pragma thumb on
void func_ov001_022cf5d4(void) {
    UnkStruct_022cf5d4 *t = data_022b4574->p;
    t->timer++;
    if (t->timer >= 0x1e) {
        t->timer = 0;
        func_02037134();
        func_ov001_022d6d4c();
    }
}
#pragma thumb reset
