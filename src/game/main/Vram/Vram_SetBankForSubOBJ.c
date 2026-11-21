#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    u32 addr[1];   /* 0x68 */
    u8 pad6C[0x26];
    u16 size;      /* 0x92 */
    u16 unk94;
    u16 unk96;
} UnkVram_02087d04;
typedef struct { u8 pad0[0x98]; u16 unk98; u16 unk9A; } UnkStruct_02088504;
extern UnkVram_02087d04 gDisplaySub;
extern UnkStruct_02088504 data_02088504;
extern u8 data_02087d6c[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void GX_DisableBankForSubOBJ(void);
void GX_SetBankForSubOBJ(s32 bank);

void Vram_SetBankForSubOBJ(s32 bank, s32 mapping, s32 mode, u16 a, u16 b) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_02087d6c, 0x14);
    gDisplaySub.size = 0;
    gDisplaySub.unk94 = 0;
    switch (bank) {
    case 8:
        gDisplaySub.unk94 = 2;
        gDisplaySub.size = 0x80;
        gDisplaySub.addr[0] = 0x06860000;
        break;
    case 0x100:
        gDisplaySub.unk94 = 0;
        gDisplaySub.size = 0x10;
        gDisplaySub.addr[0] = 0x068a0000;
        break;
    default:
        GX_DisableBankForSubOBJ();
        return;
    }
    GX_SetBankForSubOBJ(bank);
    switch (mapping) {
    case 0x10:
        gDisplaySub.unk94 = 0;
        break;
    case 0x100010:
        gDisplaySub.unk94 = 1;
        break;
    case 0x200010:
        gDisplaySub.unk94 = 2;
        break;
    default:
        gDisplaySub.unk94 = 0;
        break;
    }
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & 0xffcfffef) | mapping;
    switch (mode) {
    case 0x40:
        gDisplaySub.unk96 = 0;
        break;
    case 0x400040:
        gDisplaySub.unk96 = 1;
        break;
    default:
        gDisplaySub.unk96 = 0;
        break;
    }
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x60) | mode;
    data_02088504.unk98 = a;
    data_02088504.unk9A = b;
}
