#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7c];
    u32 slot[4];   /* 0x7c */
} UnkVram_0208922c;
extern UnkVram_0208922c data_0208922c;
extern u8 data_020892a8[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002f28(void);
void func_02003788(s32 bank);

void func_0203a590(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_020892a8, 0x10);
    switch (bank) {
    case 0x20:
        data_0208922c.slot[0] = 0x06890000;
        data_0208922c.slot[1] = 0x06892000;
        break;
    case 0x40:
        data_0208922c.slot[2] = 0x06894000;
        data_0208922c.slot[3] = 0x06896000;
        break;
    case 0x10:
        data_0208922c.slot[0] = 0x06880000;
        data_0208922c.slot[1] = 0x06882000;
        data_0208922c.slot[2] = 0x06884000;
        data_0208922c.slot[3] = 0x06886000;
        break;
    case 0x60:
        data_0208922c.slot[0] = 0x06890000;
        data_0208922c.slot[1] = 0x06892000;
        data_0208922c.slot[2] = 0x06894000;
        data_0208922c.slot[3] = 0x06896000;
        break;
    default:
        func_02002f28();
        return;
    }
    func_02003788(bank);
}
