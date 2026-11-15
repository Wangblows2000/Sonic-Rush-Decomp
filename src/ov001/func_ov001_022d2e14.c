#include <nitro/types.h>

typedef struct { u8 pad[8]; void (*fn)(void); } UnkStruct_022b4574;
extern UnkStruct_022b4574 *data_022b4574;
void func_ov001_022d0128(s32 i);
void func_ov001_022d2db8(void);

#pragma thumb on
void func_ov001_022d2e14(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        func_ov001_022d0128(i);
    }
    data_022b4574->fn = func_ov001_022d2db8;
}
#pragma thumb reset
