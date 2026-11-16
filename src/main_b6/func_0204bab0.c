#include <nitro/types.h>

typedef struct {
    u32 x0;
    u32 x4;
    u32 x8;
    u8 *xc;
    u8 *x10;
    u32 x14;
    u16 h18;
    u16 h1a;
    u32 x1c;
    u32 x20;
    u32 x24;
    u32 x28;
} UnkMap_0204bab0;

typedef struct {
    u8 pad0[0x124];
    u32 x124;
    u32 x128;
    u32 x12c;
    u32 x130;
    u16 *x134;
    u8 *x138;
} UnkGfx_0204bab0;

extern UnkGfx_0204bab0 data_ov000_0236aad8;
extern UnkMap_0204bab0 data_022c4534;

void func_02066bb0(void);

void func_0204bab0(void) {
    data_022c4534.x0 = data_ov000_0236aad8.x124;
    data_022c4534.x4 = data_ov000_0236aad8.x12c;
    data_022c4534.x8 = data_ov000_0236aad8.x130;
    data_022c4534.xc = (u8 *)data_ov000_0236aad8.x134 + 4;
    data_022c4534.x10 = data_ov000_0236aad8.x138 + 4;
    data_022c4534.x14 = data_ov000_0236aad8.x128;
    data_022c4534.h18 = data_ov000_0236aad8.x134[0];
    data_022c4534.h1a = data_ov000_0236aad8.x134[1];
    data_022c4534.x1c = 0;
    data_022c4534.x20 = 0;
    data_022c4534.x24 = data_022c4534.h18 << 6;
    data_022c4534.x28 = data_022c4534.h1a << 6;
    func_02066bb0();
}
