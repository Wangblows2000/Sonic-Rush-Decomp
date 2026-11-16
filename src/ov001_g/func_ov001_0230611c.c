#include <nitro/types.h>

typedef struct UnkStruct_0230611c {
    u8 pad0[0x5e0];
    s32 t;
    s16 unk5E4;
} UnkStruct_0230611c;

typedef struct UnkStruct_0230611c_t {
    u8 pad0[8];
    void (*func)(void);
} UnkStruct_0230611c_t;

extern UnkStruct_0230611c *volatile data_ov001_0231f5a8;
extern vu8 data_02087d14[];
extern UnkStruct_0230611c_t *data_022b4574;

void func_ov001_02305fbc(void);

void func_ov001_0230611c(void) {
    UnkStruct_0230611c *w = data_ov001_0231f5a8;
    w->t++;
    w = data_ov001_0231f5a8;
    data_02087d14[5] = data_02087d14[7] = data_02087d14[5] - w->t;
    w = data_ov001_0231f5a8;
    data_02087d14[4] = data_02087d14[6] = data_02087d14[4] + w->t;
    if (data_ov001_0231f5a8->t < 4)
        return;
    data_022b4574->func = func_ov001_02305fbc;
    data_ov001_0231f5a8->unk5E4 = 0;
}
