#include <nitro/types.h>

typedef struct UnkStruct_02305bb8_o {
    u8 pad0[0x48];
} UnkStruct_02305bb8_o;

typedef struct UnkStruct_02305bb8 {
    u32 flags;
    u8 pad4[0xc - 4];
    s32 unkC;
    u8 pad10[0x98 - 0x10];
    UnkStruct_02305bb8_o objs[8];
    u8 pad2D8[0x5ec - 0x2d8];
    s32 t;
} UnkStruct_02305bb8;

typedef struct UnkStruct_02305bb8_c {
    u8 pad0[0x3c];
    s32 unk3C;
} UnkStruct_02305bb8_c;

extern UnkStruct_02305bb8 *data_ov001_0231f5a8;
extern void *data_022b4574;

UnkStruct_02305bb8_c *func_ov001_022ffce8(void);
void func_020371c8(void *task);
void func_02032480(void *p, s32 x, s32 y, s32 z);

void func_ov001_02305bb8(void) {
    s32 c = func_ov001_022ffce8()->unk3C;

    data_ov001_0231f5a8->t += 0x199;
    if (data_ov001_0231f5a8->t >= 0x1000) {
        func_020371c8(data_022b4574);
        data_ov001_0231f5a8->unkC = 0;
        data_ov001_0231f5a8->flags |= 1;
        data_ov001_0231f5a8->t = 0x1000;
    }
    func_02032480(&data_ov001_0231f5a8->objs[c], 0x1000, data_ov001_0231f5a8->t, 0);
    func_02032480(&data_ov001_0231f5a8->objs[c + 2], 0x1000, data_ov001_0231f5a8->t, 0);
}
