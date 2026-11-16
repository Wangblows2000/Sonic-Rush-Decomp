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
extern UnkVram_02087d04 data_02087d04;
extern UnkStruct_02088504 data_02088504;
extern u8 data_02087d6c[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02002e78(void);
void func_02003290(s32 bank);

void func_0203a288(s32 bank, s32 mapping, s32 mode, u16 a, u16 b) {
    vu32 zero = 0;
    MIi_CpuClear32(zero, data_02087d6c, 0x14);
    data_02087d04.size = 0;
    data_02087d04.unk94 = 0;
    switch (bank) {
    case 8:
        data_02087d04.unk94 = 2;
        data_02087d04.size = 0x80;
        data_02087d04.addr[0] = 0x06860000;
        break;
    case 0x100:
        data_02087d04.unk94 = 0;
        data_02087d04.size = 0x10;
        data_02087d04.addr[0] = 0x068a0000;
        break;
    default:
        func_02002e78();
        return;
    }
    func_02003290(bank);
    switch (mapping) {
    case 0x10:
        data_02087d04.unk94 = 0;
        break;
    case 0x100010:
        data_02087d04.unk94 = 1;
        break;
    case 0x200010:
        data_02087d04.unk94 = 2;
        break;
    default:
        data_02087d04.unk94 = 0;
        break;
    }
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & 0xffcfffef) | mapping;
    switch (mode) {
    case 0x40:
        data_02087d04.unk96 = 0;
        break;
    case 0x400040:
        data_02087d04.unk96 = 1;
        break;
    default:
        data_02087d04.unk96 = 0;
        break;
    }
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x60) | mode;
    data_02088504.unk98 = a;
    data_02088504.unk9A = b;
}
