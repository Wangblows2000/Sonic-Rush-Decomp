#include <nitro/types.h>

typedef struct { s32 timer; } UnkStruct_022cf660;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cf660 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
void func_ov001_022cf600(void);

#pragma thumb on
void func_ov001_022cf660(void) {
    UnkStruct_022cf660 *t = data_022b4574->p;
    t->timer++;
    if (t->timer >= 0x78) {
        t->timer = 0;
        data_022b4574->fn = func_ov001_022cf600;
    }
}
#pragma thumb reset
