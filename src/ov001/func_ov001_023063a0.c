#include <nitro/types.h>

typedef struct { u8 pad[4]; void *tasks[5]; } UnkStruct_0231f5a8;
extern UnkStruct_0231f5a8 *data_ov001_0231f5a8;
extern u32 data_02087cb8;
extern s32 data_02087d04[];
void func_020371c8(void *p);

void func_ov001_023063a0(void) {
    s32 i;
    for (i = 0; i < 5; i++) {
        if (data_ov001_0231f5a8->tasks[i] != NULL) {
            func_020371c8(data_ov001_0231f5a8->tasks[i]);
            data_ov001_0231f5a8->tasks[i] = NULL;
        }
    }
    *(vu32 *)0x04001000 &= ~0x1f00;
    data_02087d04[7] = 0;
    data_02087cb8 &= ~0x400;
}
