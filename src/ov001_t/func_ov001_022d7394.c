#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022d7394;

typedef struct {
    u8 pad0[0x25c];
    UnkSpr_022d7394 spr[19];
} UnkStruct_022d7394;

extern u16 data_02087d04[];

#pragma thumb on
void func_ov001_022d7394(UnkStruct_022d7394 *p) {
    s32 i;

    *(vu16 *)0x4001008 = (*(vu16 *)0x4001008 & ~3) | 0;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & ~3) | 1;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & ~3) | 2;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & ~3) | 3;
    for (i = 0; i < 19; i++) {
        p->spr[i].flags &= ~0x60000;
    }
    data_02087d04[4] = 0x100;
    data_02087d04[5] = 0;
    *(vu32 *)0x4001018 = (data_02087d04[4] & 0x1ff) | ((data_02087d04[5] << 16) & 0x1ff0000);
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & 0x43) | 0x4088;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1f00;
}
#pragma thumb reset
