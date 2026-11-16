#include <nitro/types.h>

typedef struct {
    u32 addr[4];
    u32 ext[6];    /* 0x10 */
    u8 count;      /* 0x28 */
    u8 extCount;   /* 0x29 */
} UnkVram_0208a754;
extern UnkVram_0208a754 data_0208a754;
extern u8 data_0208a764[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002edc(void);
void func_020033d8(s32 bank);

void Vram_SetBankForTexPltt(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_0208a764, 0x18);
    switch (bank) {
    case 0x20:
        data_0208a754.ext[0] = 0x06890000;
        data_0208a754.extCount = 1;
        break;
    case 0x40:
        data_0208a754.ext[0] = 0x06894000;
        data_0208a754.extCount = 1;
        break;
    case 0x60:
        data_0208a754.ext[0] = 0x06890000;
        data_0208a754.ext[1] = 0x06894000;
        data_0208a754.extCount = 2;
        break;
    case 0x10:
        data_0208a754.ext[0] = 0x06880000;
        data_0208a754.ext[1] = 0x06884000;
        data_0208a754.ext[2] = 0x06888000;
        data_0208a754.ext[3] = 0x0688c000;
        data_0208a754.extCount = 4;
        break;
    case 0x30:
        data_0208a754.ext[0] = 0x06880000;
        data_0208a754.ext[1] = 0x06884000;
        data_0208a754.ext[2] = 0x06888000;
        data_0208a754.ext[3] = 0x0688c000;
        data_0208a754.ext[4] = 0x06890000;
        data_0208a754.extCount = 5;
        break;
    case 0x70:
        data_0208a754.ext[0] = 0x06880000;
        data_0208a754.ext[1] = 0x06884000;
        data_0208a754.ext[2] = 0x06888000;
        data_0208a754.ext[3] = 0x0688c000;
        data_0208a754.ext[4] = 0x06890000;
        data_0208a754.ext[5] = 0x06894000;
        data_0208a754.extCount = 6;
        break;
    default:
        func_02002edc();
        return;
    }
    func_020033d8(bank);
}
