#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022d7454;

typedef struct {
    u8 pad0[0x25c];
    UnkSpr_022d7454 spr[19];
} UnkStruct_022d7454;

extern u16 data_02087d04[];

#pragma thumb on
void func_ov001_022d7454(UnkStruct_022d7454 *p) {
    s32 i;

    *(vu16 *)0x4001008 = (*(vu16 *)0x4001008 & ~3) | 1;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & ~3) | 2;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & ~3) | 0;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & ~3) | 3;
    for (i = 0; i < 19; i++) {
        p->spr[i].flags &= ~0x60000;
        p->spr[i].flags |= 0x20000;
    }
    data_02087d04[4] = 0;
    data_02087d04[5] = 0;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & 0x43) | 0x4390;
}
#pragma thumb reset
