#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    u32 addr[3];   /* 0x68 */
    u8 pad74[0x1e];
    u16 size;      /* 0x92 */
    u16 unk94;
    u16 unk96;
} UnkVram_0208922c;
typedef struct { u8 pad0[0x98]; u16 unk98; u16 unk9A; } UnkStruct_02089a2c;
extern UnkVram_0208922c gDisplayMain;
extern UnkStruct_02089a2c data_02089a2c;
extern u8 data_02089294[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002f4c(void);
void func_0200389c(s32 bank);

void Vram_SetBankForOBJ(s32 bank, s32 mapping, s32 mode, u16 a, u16 b) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_02089294, 0x14);
    gDisplayMain.size = 0;
    gDisplayMain.unk94 = 0;
    switch (bank) {
    case 0x20:
        gDisplayMain.unk94 = 0;
        gDisplayMain.size = 0x10;
        gDisplayMain.addr[0] = 0x06890000;
        break;
    case 0x40:
        gDisplayMain.unk94 = 0;
        gDisplayMain.size = 0x10;
        gDisplayMain.addr[0] = 0x06894000;
        break;
    case 0x60:
        gDisplayMain.unk94 = 0;
        gDisplayMain.size = 0x20;
        gDisplayMain.addr[0] = 0x06890000;
        gDisplayMain.addr[1] = 0x06894000;
        break;
    case 0x10:
        gDisplayMain.unk94 = 1;
        gDisplayMain.size = 0x40;
        gDisplayMain.addr[0] = 0x06880000;
        break;
    case 0x70:
        gDisplayMain.unk94 = 1;
        gDisplayMain.size = 0x60;
        gDisplayMain.addr[0] = 0x06880000;
        gDisplayMain.addr[1] = 0x06890000;
        gDisplayMain.addr[2] = 0x06894000;
        break;
    case 1:
        gDisplayMain.unk94 = 2;
        gDisplayMain.size = 0x80;
        gDisplayMain.addr[0] = 0x06800000;
        break;
    case 2:
        gDisplayMain.unk94 = 2;
        gDisplayMain.size = 0x80;
        gDisplayMain.addr[0] = 0x06820000;
        break;
    case 3:
        gDisplayMain.unk94 = 3;
        gDisplayMain.size = 0x100;
        gDisplayMain.addr[0] = 0x06800000;
        gDisplayMain.addr[1] = 0x06820000;
        break;
    default:
        func_02002f4c();
        return;
    }
    func_0200389c(bank);
    switch (mapping) {
    case 0x10:
        gDisplayMain.unk94 = 0;
        break;
    case 0x100010:
        gDisplayMain.unk94 = 1;
        break;
    case 0x200010:
        gDisplayMain.unk94 = 2;
        break;
    case 0x300010:
        gDisplayMain.unk94 = 3;
        break;
    default:
        gDisplayMain.unk94 = 0;
        break;
    }
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & 0xffcfffef) | mapping;
    switch (mode) {
    case 0x40:
        gDisplayMain.unk96 = 0;
        break;
    case 0x400040:
        gDisplayMain.unk96 = 1;
        break;
    default:
        gDisplayMain.unk96 = 0;
        break;
    }
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & 0xffbfff9f) | mode;
    data_02089a2c.unk98 = a;
    data_02089a2c.unk9A = b;
}
