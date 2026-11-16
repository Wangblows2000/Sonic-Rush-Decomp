#include <nitro/types.h>

typedef struct {
    u8 pad0[0x4c];
    u32 addr[4];   /* 0x4c */
    u8 pad5C[0x34];
    u16 size;      /* 0x90 */
} UnkVram_0208922c;
extern UnkVram_0208922c data_0208922c;
extern u8 data_02089278[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002f60(void);
void func_02003a08(s32 bank);

void func_0203a9d0(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_02089278, 0x1c);
    data_0208922c.size = 0;
    switch (bank) {
    case 0x20:
        data_0208922c.size = 0x10;
        data_0208922c.addr[0] = 0x06890000;
        break;
    case 0x40:
        data_0208922c.size = 0x10;
        data_0208922c.addr[0] = 0x06894000;
        break;
    case 0x60:
        data_0208922c.size = 0x20;
        data_0208922c.addr[0] = 0x06894000;
        data_0208922c.addr[1] = 0x06890000;
        break;
    case 0x10:
        data_0208922c.size = 0x40;
        data_0208922c.addr[0] = 0x06880000;
        break;
    case 0x70:
        data_0208922c.size = 0x60;
        data_0208922c.addr[0] = 0x06880000;
        data_0208922c.addr[1] = 0x06890000;
        data_0208922c.addr[2] = 0x06894000;
        break;
    case 1:
        data_0208922c.size = 0x80;
        data_0208922c.addr[0] = 0x06800000;
        break;
    case 2:
        data_0208922c.size = 0x80;
        data_0208922c.addr[0] = 0x06820000;
        break;
    case 4:
        data_0208922c.size = 0x80;
        data_0208922c.addr[0] = 0x06840000;
        break;
    case 8:
        data_0208922c.size = 0x80;
        data_0208922c.addr[0] = 0x06860000;
        break;
    case 3:
        data_0208922c.size = 0x100;
        data_0208922c.addr[0] = 0x06800000;
        data_0208922c.addr[1] = 0x06820000;
        break;
    case 6:
        data_0208922c.size = 0x100;
        data_0208922c.addr[0] = 0x06820000;
        data_0208922c.addr[1] = 0x06840000;
        break;
    case 12:
        data_0208922c.size = 0x100;
        data_0208922c.addr[0] = 0x06840000;
        data_0208922c.addr[1] = 0x06860000;
        break;
    case 7:
        data_0208922c.size = 0x180;
        data_0208922c.addr[0] = 0x06800000;
        data_0208922c.addr[1] = 0x06820000;
        data_0208922c.addr[2] = 0x06840000;
        break;
    case 14:
        data_0208922c.size = 0x180;
        data_0208922c.addr[0] = 0x06820000;
        data_0208922c.addr[1] = 0x06840000;
        data_0208922c.addr[2] = 0x06860000;
        break;
    case 15:
        data_0208922c.size = 0x200;
        data_0208922c.addr[0] = 0x06800000;
        data_0208922c.addr[1] = 0x06820000;
        data_0208922c.addr[2] = 0x06840000;
        data_0208922c.addr[3] = 0x06860000;
        break;
    default:
        func_02002f60();
        return;
    }
    func_02003a08(bank);
}
