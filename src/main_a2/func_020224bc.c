#include <nitro/types.h>

typedef struct { u32 w[0x99]; } UnkGlb_020224bc;

extern UnkGlb_020224bc data_02084d38;
extern u8 data_02084d84[];
extern u8 data_02084d40[];
extern u8 data_02084df4[];

void func_0200148c(void *m);
void func_02001cb8(void *m);
void func_02000dfc(void *m);

void func_020224bc(void) {
    data_02084d38.w[0x00 / 4] = 0x17101610;
    data_02084d38.w[0x04 / 4] = 0;
    data_02084d38.w[0x48 / 4] = 2;
    data_02084d38.w[0x7C / 4] = 0x32323232;
    data_02084d38.w[0x90 / 4] = 0x60293130;
    data_02084d38.w[0xA4 / 4] = 0x33333333;
    data_02084d38.w[0xB8 / 4] = 0x002A1B19;
    func_0200148c(data_02084d84);
    func_02001cb8(data_02084d40);
    data_02084d38.w[0x80 / 4] = 0x2D8B62D8;
    data_02084d38.w[0x84 / 4] = 0x40000200;
    data_02084d38.w[0x88 / 4] = 0x800001FF;
    data_02084d38.w[0x8C / 4] = 0xC0080000;
    data_02084d38.w[0x94 / 4] = 0x4210C210;
    data_02084d38.w[0x98 / 4] = 0x4210C210;
    data_02084d38.w[0x9C / 4] = 0x001F008F;
    data_02084d38.w[0xA0 / 4] = 0xBFFF0000;
    data_02084d38.w[0xA8 / 4] = 0x00007FFF;
    data_02084d38.w[0xAC / 4] = 0x4000001F;
    data_02084d38.w[0xB0 / 4] = 0x800003E0;
    data_02084d38.w[0xB4 / 4] = 0xC0007C00;
    data_02084d38.w[0xE0 / 4] = 0;
    data_02084d38.w[0xE4 / 4] = 0;
    data_02084d38.w[0xE8 / 4] = 0;
    func_02000dfc(data_02084df4);
    data_02084d38.w[0xEC / 4] = 0x1000;
    data_02084d38.w[0xF0 / 4] = 0x1000;
    data_02084d38.w[0xF4 / 4] = 0x1000;
    data_02084d38.w[0xF8 / 4] = 0;
    data_02084d38.w[0xFC / 4] = 0;
    data_02084d38.w[0x248 / 4] = 0;
    data_02084d38.w[0x244 / 4] = 0;
    data_02084d38.w[0x240 / 4] = 0;
    data_02084d38.w[0x254 / 4] = 0;
    data_02084d38.w[0x24C / 4] = 0;
    data_02084d38.w[0x250 / 4] = 0x1000;
    data_02084d38.w[0x25C / 4] = 0;
    data_02084d38.w[0x258 / 4] = 0;
    data_02084d38.w[0x260 / 4] = -0x1000;
}
