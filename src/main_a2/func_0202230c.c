#include <nitro/types.h>

typedef struct { u8 unk0[0x7C]; u32 unk7C; u8 unk80[0x7C]; u32 flag; } UnkGlb_0202230c;

extern UnkGlb_0202230c data_02084d38;
extern u32 data_02084d3c[];
extern u32 data_02084d84[];
extern u32 data_02084df4[];
extern u32 data_02084db4[];
extern u32 data_02084e30[];

void func_02025268(u32 op, const void *args, u32 n);
void func_020010d0(const void *src, void *dst);

void func_0202230c(void) {
    u32 arg;
    u32 mtx[12];
    func_02025268(0x1610, data_02084d3c, 0x11);
    func_02025268(0x19, data_02084d84, 0xC);
    func_02025268(0x1B19, data_02084df4, 0xF);
    arg = 2;
    func_02025268(0x10, &arg, 1);
    func_020010d0(data_02084df4, mtx);
    func_02025268(0x17, mtx, 0xC);
    func_02025268(data_02084d38.unk7C, (u8 *)data_02084db4 + 4, 0xE);
    func_02025268(0x15, NULL, 0);
    func_02025268(0x2A, data_02084e30, 1);
    data_02084d38.flag |= 1;
    data_02084d38.flag &= ~2;
}
