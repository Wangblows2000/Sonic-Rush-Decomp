#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7c];
    u32 slot[4];   /* 0x7c */
} UnkVram_0208922c;
extern UnkVram_0208922c gDisplayMain;
extern u8 data_020892a8[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void GX_DisableBankForBGExtPltt(void);
void GX_SetBankForBGExtPltt(s32 bank);

void Vram_SetBankForBGExtPltt(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_020892a8, 0x10);
    switch (bank) {
    case 0x20:
        gDisplayMain.slot[0] = 0x06890000;
        gDisplayMain.slot[1] = 0x06892000;
        break;
    case 0x40:
        gDisplayMain.slot[2] = 0x06894000;
        gDisplayMain.slot[3] = 0x06896000;
        break;
    case 0x10:
        gDisplayMain.slot[0] = 0x06880000;
        gDisplayMain.slot[1] = 0x06882000;
        gDisplayMain.slot[2] = 0x06884000;
        gDisplayMain.slot[3] = 0x06886000;
        break;
    case 0x60:
        gDisplayMain.slot[0] = 0x06890000;
        gDisplayMain.slot[1] = 0x06892000;
        gDisplayMain.slot[2] = 0x06894000;
        gDisplayMain.slot[3] = 0x06896000;
        break;
    default:
        GX_DisableBankForBGExtPltt();
        return;
    }
    GX_SetBankForBGExtPltt(bank);
}
