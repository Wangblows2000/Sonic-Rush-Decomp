#include <nitro/types.h>

typedef struct {
    u8 pad0[0x8c];
    u32 tex;   /* 0x8c */
} UnkVram_0208922c;
extern UnkVram_0208922c gDisplayMain;
extern u8 data_020892b8[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void GX_DisableBankForOBJExtPltt(void);
void GX_SetBankForOBJExtPltt(s32 bank);

void Vram_SetBankForOBJExtPltt(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_020892b8, 4);
    switch (bank) {
    case 0x20:
        gDisplayMain.tex = 0x06890000;
        break;
    case 0x40:
        gDisplayMain.tex = 0x06894000;
        break;
    default:
        GX_DisableBankForOBJExtPltt();
        return;
    }
    GX_SetBankForOBJExtPltt(bank);
}
