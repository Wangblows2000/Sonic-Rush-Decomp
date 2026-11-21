#include <nitro/types.h>

typedef struct {
    u8 pad0[0x4c];
    u32 addr[4];   /* 0x4c */
    u8 pad5C[0x34];
    u16 size;      /* 0x90 */
} UnkVram_0208922c;
extern UnkVram_0208922c gDisplayMain;
extern u8 data_02089278[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void GX_DisableBankForBG(void);
void GX_SetBankForBG(s32 bank);

void Vram_SetBankForBG(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_02089278, 0x1c);
    gDisplayMain.size = 0;
    switch (bank) {
    case 0x20:
        gDisplayMain.size = 0x10;
        gDisplayMain.addr[0] = 0x06890000;
        break;
    case 0x40:
        gDisplayMain.size = 0x10;
        gDisplayMain.addr[0] = 0x06894000;
        break;
    case 0x60:
        gDisplayMain.size = 0x20;
        gDisplayMain.addr[0] = 0x06894000;
        gDisplayMain.addr[1] = 0x06890000;
        break;
    case 0x10:
        gDisplayMain.size = 0x40;
        gDisplayMain.addr[0] = 0x06880000;
        break;
    case 0x70:
        gDisplayMain.size = 0x60;
        gDisplayMain.addr[0] = 0x06880000;
        gDisplayMain.addr[1] = 0x06890000;
        gDisplayMain.addr[2] = 0x06894000;
        break;
    case 1:
        gDisplayMain.size = 0x80;
        gDisplayMain.addr[0] = 0x06800000;
        break;
    case 2:
        gDisplayMain.size = 0x80;
        gDisplayMain.addr[0] = 0x06820000;
        break;
    case 4:
        gDisplayMain.size = 0x80;
        gDisplayMain.addr[0] = 0x06840000;
        break;
    case 8:
        gDisplayMain.size = 0x80;
        gDisplayMain.addr[0] = 0x06860000;
        break;
    case 3:
        gDisplayMain.size = 0x100;
        gDisplayMain.addr[0] = 0x06800000;
        gDisplayMain.addr[1] = 0x06820000;
        break;
    case 6:
        gDisplayMain.size = 0x100;
        gDisplayMain.addr[0] = 0x06820000;
        gDisplayMain.addr[1] = 0x06840000;
        break;
    case 12:
        gDisplayMain.size = 0x100;
        gDisplayMain.addr[0] = 0x06840000;
        gDisplayMain.addr[1] = 0x06860000;
        break;
    case 7:
        gDisplayMain.size = 0x180;
        gDisplayMain.addr[0] = 0x06800000;
        gDisplayMain.addr[1] = 0x06820000;
        gDisplayMain.addr[2] = 0x06840000;
        break;
    case 14:
        gDisplayMain.size = 0x180;
        gDisplayMain.addr[0] = 0x06820000;
        gDisplayMain.addr[1] = 0x06840000;
        gDisplayMain.addr[2] = 0x06860000;
        break;
    case 15:
        gDisplayMain.size = 0x200;
        gDisplayMain.addr[0] = 0x06800000;
        gDisplayMain.addr[1] = 0x06820000;
        gDisplayMain.addr[2] = 0x06840000;
        gDisplayMain.addr[3] = 0x06860000;
        break;
    default:
        GX_DisableBankForBG();
        return;
    }
    GX_SetBankForBG(bank);
}
