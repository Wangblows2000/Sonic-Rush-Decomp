#include <nitro/types.h>

extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;
void func_0203344c(void *p, s32 a, s32 b);
void func_02032480(void *p, s32 a, s32 b, s32 c);
void func_02032d2c(void *p);

void func_ov001_02300978(u8 *p, u8 *o) {
    *(s16 *)(o + 4) = (data_ov001_0231f524 + (*(s32 *)(p + 0x18) + *(s32 *)(p + 0x30))) >> 12;
    *(s16 *)(o + 6) = (data_ov001_0231f520 + (*(s32 *)(p + 0x1c) + *(s32 *)(p + 0x34))) >> 12;
    *(u32 *)(o + 0x2c) &= ~0x180;
    if (!(*(u32 *)(p + 0x14) & 0x40)) {
        *(u32 *)(o + 0x2c) |= 0x80;
    }
    if (!(*(u32 *)(p + 0x14) & 0x10000)) {
        func_0203344c(o, 0, 0);
    }
    if (*(s32 *)(p + 0x44) != 0x1000 || *(s32 *)(p + 0x48) != 0x1000 || *(u16 *)(p + 0x40) != 0) {
        func_02032480(o, *(s32 *)(p + 0x44), *(s32 *)(p + 0x48), *(u16 *)(p + 0x40));
        return;
    }
    func_02032d2c(o);
}
