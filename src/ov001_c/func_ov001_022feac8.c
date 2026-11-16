#include <nitro/types.h>

typedef struct UnkStruct_022feac8 {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[0x338 - 8];
    u16 unk338;
} UnkStruct_022feac8;

typedef struct UnkStruct_022feac8_g {
    u8 pad0[0x320];
    s32 unk320;
} UnkStruct_022feac8_g;

typedef struct UnkStruct_022feac8_oam {
    u32 attr01;
    u16 attr2;
} UnkStruct_022feac8_oam;

extern UnkStruct_022feac8 *data_ov001_0231f52c;

void func_020030bc(void);
void func_020030a8(void);
void Vram_SetBankForSubOBJ(s32 bank, s32 mapping, s32 mode, u16 a, u16 b);
void GX_SetBankForLCDC(u32 bank);
void Vram_SetBankForSubBG(s32 bank);
UnkStruct_022feac8_g *func_ov001_022ffce8(void);
UnkStruct_022feac8_oam *Oam_AllocObj(u32 idx, u32 prio);
s32 func_02002094(s32 n, s32 d);

void func_ov001_022feac8(void) {
    s32 x;
    s32 y;
    UnkStruct_022feac8_oam *o;
    u16 t;

    data_ov001_0231f52c->unk338 ^= 1;
    if (data_ov001_0231f52c->unk338 != 0) {
        *(vu16 *)0x4000304 &= ~0x8000;
        func_020030bc();
        Vram_SetBankForSubOBJ(8, 0x100010, 0x20, 0, 0x100);
        GX_SetBankForLCDC(4);
        *(vu32 *)0x4000064 = 0x80320010;
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1000;
        data_ov001_0231f52c->unk4 = 1;
        func_ov001_022ffce8()->unk320 = 0x60000;
    } else {
        *(vu16 *)0x4000304 |= 0x8000;
        func_020030a8();
        Vram_SetBankForSubBG(4);
        GX_SetBankForLCDC(8);
        *(vu32 *)0x4000064 = 0x80330010;
        *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x400;
        *(vu16 *)0x400100c = (*(vu16 *)0x400100c & 0x43) | 0x4084;
        for (y = 0; y < 0xc0; y += 0x40) {
            for (x = 0; x < 0x100; x += 0x40) {
                o = Oam_AllocObj(1, 0);
                t = (func_02002094(x, 8) + (func_02002094(y, 8) << 5)) | 0xf000;
                o->attr01 = ((y & 0xff) | 0xc0000c00) | ((x & 0x1ff) << 16);
                o->attr2 = t;
            }
        }
        data_ov001_0231f52c->unk4 = 2;
        func_ov001_022ffce8()->unk320 = 0x120000;
    }
}
