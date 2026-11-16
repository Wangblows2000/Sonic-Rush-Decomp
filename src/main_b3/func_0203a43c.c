#include <nitro/types.h>

typedef struct {
    u8 pad0[0x4c];
    u32 addr[2];   /* 0x4c */
    u8 pad54[0x3c];
    u16 size;      /* 0x90 */
} UnkVram_02087d04;
extern UnkVram_02087d04 data_02087d04;
extern u8 data_02087d50[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002e8c(void);
void func_0200330c(s32 bank);

void func_0203a43c(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_02087d50, 0x1c);
    data_02087d04.size = 0;
    switch (bank) {
    case 4:
        data_02087d04.size = 0x80;
        data_02087d04.addr[0] = 0x06840000;
        break;
    case 0x80:
        data_02087d04.size = 0x20;
        data_02087d04.addr[0] = 0x06898000;
        break;
    case 0x180:
        data_02087d04.size = 0x30;
        data_02087d04.addr[0] = 0x06898000;
        data_02087d04.addr[1] = 0x068a0000;
        break;
    default:
        func_02002e8c();
        return;
    }
    func_0200330c(bank);
}
