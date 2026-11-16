#include <nitro/types.h>

typedef struct { s32 timer; } UnkStruct_022cf660;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cf660 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void func_ov001_022cf600(void);

#pragma thumb on
void func_ov001_022cf660(void) {
    UnkStruct_022cf660 *t = gTaskCurrent->p;
    t->timer++;
    if (t->timer >= 0x78) {
        t->timer = 0;
        gTaskCurrent->fn = func_ov001_022cf600;
    }
}
#pragma thumb reset
