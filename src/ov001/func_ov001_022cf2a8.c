#include <nitro/types.h>

typedef struct { s32 v; } UnkStruct_022cf2a8;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cf2a8 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
BOOL func_02060e8c(void);
void func_ov001_022cf0e4(void);

#pragma thumb on
void func_ov001_022cf2a8(void) {
    if (!func_02060e8c()) {
        data_022b4574->p->v = 0;
        data_022b4574->fn = func_ov001_022cf0e4;
    }
}
#pragma thumb reset
