#include <nitro/types.h>

typedef struct { u32 reg; void *a; void *b; s32 c; s32 d; } UnkStruct_02087cdc;
extern u8 *data_ov001_0231f5a8;
extern u32 data_02087cb8;
extern UnkStruct_02087cdc data_02087cdc;
void *func_020372d0(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov001_02305ec8(void);

void func_ov001_02305f08(void) {
    *(void **)(data_ov001_0231f5a8 + 0x10) = func_020372d0(func_ov001_02305ec8, 0, 0, 0x3100, 2, 0);
    *(u16 *)(data_ov001_0231f5a8 + 0x5dc) = 0;
    *(u16 *)(data_ov001_0231f5a8 + 0x5de) = 0;
    data_02087cdc.reg = 0x04001014;
    data_02087cdc.a = data_ov001_0231f5a8 + 0x2d8;
    data_02087cdc.b = data_ov001_0231f5a8 + 0x458;
    data_02087cdc.c = 2;
    data_02087cdc.d = 0;
    data_02087cb8 |= 0x400;
}
