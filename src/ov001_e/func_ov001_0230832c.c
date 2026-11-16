#include <nitro/types.h>

typedef struct UnkStruct_0230832c_o {
    u8 pad0[0x48];
} UnkStruct_0230832c_o;

typedef struct UnkStruct_0230832c {
    u8 pad0[0x270];
    UnkStruct_0230832c_o a;
    UnkStruct_0230832c_o b;
    UnkStruct_0230832c_o c;
} UnkStruct_0230832c;

extern UnkStruct_0230832c *data_ov001_0231f5bc;

s32 func_ov001_02314368(void);
void func_02033550(UnkStruct_0230832c_o *s, u16 index);

void func_ov001_0230832c(s32 t) {
    UnkStruct_0230832c *g = data_ov001_0231f5bc;
    s32 n = func_ov001_02314368();
    s32 d100 = n / 100 % 10;
    s32 d10 = n / 10 % 10;
    s32 d1 = n % 10;

    switch (t) {
    case 0:
        func_02033550(&g->a, d100 + 0x1e);
        break;
    case 0x14:
        func_02033550(&g->b, d10 + 0x1e);
        break;
    case 0x28:
        func_02033550(&g->c, d1 + 0x1e);
        break;
    }
}
