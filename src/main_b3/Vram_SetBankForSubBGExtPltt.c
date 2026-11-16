#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7c];
    u32 slot[4];   /* 0x7c */
} UnkVram_02087d04;
extern UnkVram_02087d04 gDisplaySub;
extern u8 data_02087d80[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void GX_DisableBankForSubBGExtPltt(void);
void GX_SetBankForSubBGExtPltt(s32 bank);

void Vram_SetBankForSubBGExtPltt(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_02087d80, 0x10);
    switch (bank) {
    case 0x80:
        gDisplaySub.slot[0] = 0x06898000;
        gDisplaySub.slot[1] = 0x0689a000;
        gDisplaySub.slot[2] = 0x0689c000;
        gDisplaySub.slot[3] = 0x0689e000;
        break;
    default:
        GX_DisableBankForSubBGExtPltt();
        return;
    }
    GX_SetBankForSubBGExtPltt(bank);
}
