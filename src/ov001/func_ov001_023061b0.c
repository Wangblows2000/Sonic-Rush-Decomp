#include <nitro/types.h>

typedef struct { u8 pad[8]; void (*fn)(void); } UnkStruct_022b4574;
typedef struct { u8 pad[0x5e0]; s32 step; } UnkStruct_0231f5a8;
typedef struct { u8 b0; u8 pad[0xb]; u32 wc; } UnkStruct_02087d14;
extern UnkStruct_022b4574 *data_022b4574;
extern UnkStruct_0231f5a8 *data_ov001_0231f5a8;
extern UnkStruct_02087d14 data_02087d14;
s32 func_02002094(s32 a, s32 b);
void func_ov001_0230611c(void);
void func_ov001_02305f08(void);

void func_ov001_023061b0(void) {
    UnkStruct_02087d14 *d = &data_02087d14;
    data_ov001_0231f5a8->step--;
    if (data_ov001_0231f5a8->step > 0) {
        d->b0 = func_02002094((0x10 - data_ov001_0231f5a8->step) << 8, 0x10);
        return;
    }
    data_022b4574->fn = func_ov001_0230611c;
    d->b0 = 0xff;
    d->wc |= 2;
    data_ov001_0231f5a8->step = 0;
    func_ov001_02305f08();
}
