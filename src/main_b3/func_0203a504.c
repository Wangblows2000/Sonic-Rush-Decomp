#include <nitro/types.h>

typedef struct {
    u8 pad0[0x8c];
    u32 tex;   /* 0x8c */
} UnkVram_0208922c;
extern UnkVram_0208922c data_0208922c;
extern u8 data_020892b8[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002f04(void);
void func_020036d0(s32 bank);

void func_0203a504(s32 bank) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_020892b8, 4);
    switch (bank) {
    case 0x20:
        data_0208922c.tex = 0x06890000;
        break;
    case 0x40:
        data_0208922c.tex = 0x06894000;
        break;
    default:
        func_02002f04();
        return;
    }
    func_020036d0(bank);
}
