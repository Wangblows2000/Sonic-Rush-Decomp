#include <nitro/types.h>

typedef struct {
    u32 addr[4];
    u8 pad10[0x18];
    u8 count;      /* 0x28 */
} UnkVram_0208a754;
extern UnkVram_0208a754 data_0208a754;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002ef0(void);
void func_020034cc(s32 bank);

void func_02039ef8(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, &data_0208a754, 0x10);
    switch (bank) {
    case 1:
        data_0208a754.addr[0] = 0x06800000;
        data_0208a754.count = 1;
        break;
    case 2:
        data_0208a754.addr[0] = 0x06820000;
        data_0208a754.count = 1;
        break;
    case 4:
        data_0208a754.addr[0] = 0x06840000;
        data_0208a754.count = 1;
        break;
    case 8:
        data_0208a754.addr[0] = 0x06860000;
        data_0208a754.count = 1;
        break;
    case 3:
        data_0208a754.addr[0] = 0x06800000;
        data_0208a754.addr[1] = 0x06820000;
        data_0208a754.count = 2;
        break;
    case 6:
        data_0208a754.addr[0] = 0x06820000;
        data_0208a754.addr[1] = 0x06840000;
        data_0208a754.count = 2;
        break;
    case 12:
        data_0208a754.addr[0] = 0x06840000;
        data_0208a754.addr[1] = 0x06860000;
        data_0208a754.count = 2;
        break;
    case 7:
        data_0208a754.addr[0] = 0x06800000;
        data_0208a754.addr[1] = 0x06820000;
        data_0208a754.addr[2] = 0x06840000;
        data_0208a754.count = 3;
        break;
    case 14:
        data_0208a754.addr[0] = 0x06820000;
        data_0208a754.addr[1] = 0x06840000;
        data_0208a754.addr[2] = 0x06860000;
        data_0208a754.count = 3;
        break;
    case 15:
        data_0208a754.addr[0] = 0x06800000;
        data_0208a754.addr[1] = 0x06820000;
        data_0208a754.addr[2] = 0x06840000;
        data_0208a754.addr[3] = 0x06860000;
        data_0208a754.count = 4;
        break;
    case 5:
        data_0208a754.addr[0] = 0x06800000;
        data_0208a754.addr[1] = 0x06840000;
        data_0208a754.count = 2;
        break;
    case 9:
        data_0208a754.addr[0] = 0x06800000;
        data_0208a754.addr[1] = 0x06860000;
        data_0208a754.count = 2;
        break;
    case 10:
        data_0208a754.addr[0] = 0x06820000;
        data_0208a754.addr[1] = 0x06860000;
        data_0208a754.count = 2;
        break;
    case 11:
        data_0208a754.addr[0] = 0x06800000;
        data_0208a754.addr[1] = 0x06820000;
        data_0208a754.addr[2] = 0x06860000;
        data_0208a754.count = 3;
        break;
    case 13:
        data_0208a754.addr[0] = 0x06800000;
        data_0208a754.addr[1] = 0x06840000;
        data_0208a754.addr[2] = 0x06860000;
        data_0208a754.count = 3;
        break;
    default:
        func_02002ef0();
        return;
    }
    func_020034cc(bank);
}
