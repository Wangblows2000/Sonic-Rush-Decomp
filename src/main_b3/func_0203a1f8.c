#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7c];
    u32 slot[4];   /* 0x7c */
} UnkVram_02087d04;
extern UnkVram_02087d04 data_02087d04;
extern u8 data_02087d80[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002e50(void);
void func_02003204(s32 bank);

void func_0203a1f8(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_02087d80, 0x10);
    switch (bank) {
    case 0x80:
        data_02087d04.slot[0] = 0x06898000;
        data_02087d04.slot[1] = 0x0689a000;
        data_02087d04.slot[2] = 0x0689c000;
        data_02087d04.slot[3] = 0x0689e000;
        break;
    default:
        func_02002e50();
        return;
    }
    func_02003204(bank);
}
