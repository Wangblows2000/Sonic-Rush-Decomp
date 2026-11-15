#include <nitro/types.h>

typedef struct UnkStruct_0230c364_c0 {
    u8 pad0[4];
    s32 unk4;
    u32 unk8;
    u8 padC[0xc];
    s32 unk18;
    s32 unk1C;
} UnkStruct_0230c364_c0;

typedef struct UnkStruct_0230c364_c4s {
    u8 pad[8];
    s32 unk8;
    s32 unkC;
} UnkStruct_0230c364_c4s;

typedef struct UnkStruct_0230c364_c4 {
    u8 pad0[0x2c];
    UnkStruct_0230c364_c4s *unk2C;
} UnkStruct_0230c364_c4;

typedef struct UnkStruct_0230c364_bl {
    u16 a;
    u16 b;
} UnkStruct_0230c364_bl;

typedef struct UnkStruct_0230c364_br {
    UnkStruct_0230c364_bl unk0[4];
    u8 pad10[0x10];
    u16 unk20;
    u8 pad22[0x26];
    u16 unk48;
} UnkStruct_0230c364_br;

typedef struct UnkStruct_0230c364_f754 {
    u8 pad[0x80];
    u32 unk80;
    u32 unk84;
} UnkStruct_0230c364_f754;

typedef struct UnkStruct_0230c364_4560 {
    u8 pad[0x28];
    u16 unk28;
} UnkStruct_0230c364_4560;

extern void *data_0208fe80;
extern UnkStruct_0230c364_c0 *data_ov001_0231f5c0;
extern UnkStruct_0230c364_c4 *data_ov001_0231f5c4;
extern UnkStruct_0230c364_4560 data_022c4560;
extern UnkStruct_0230c364_br data_0208922c;
extern UnkStruct_0230c364_br data_02087d04;
extern UnkStruct_0230c364_f754 data_0208f754;
extern u8 data_ov001_023171a0[];

u8 *func_02034a40(void *heap, u32 size, s32 align);
u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);
void func_0203a9d0(s32 a);
void func_0203a6bc(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a590(s32 a);
void func_0203a504(s32 a);
void func_02039ef8(s32 a);
void func_02039d40(s32 a);
void func_02002ec8(void);
void func_0203a43c(s32 a);
void func_0203a288(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a1f8(s32 a);
void func_0203a18c(s32 a);
void func_02002eb4(void);
void func_02002ea0(void);
void func_02039d08(s32 i);
void func_02039b48(void);
void func_020395f4(void);
void func_02002ab0(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(u32 mode);
void GX_DispOn(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 b);
void func_02004040(u32 a, u32 b, u16 c, u32 d, BOOL e);
void func_02022270(u32 index, u32 value);
void func_02022284(u32 idx, s32 x, s32 y, s32 z);
void func_02004084(const void *src);
void func_ov001_0230be84(UnkStruct_0230c364_c4 *p);
void func_ov001_0230bca8(void);
void func_ov001_0230b848(void);
void func_ov001_02314874(void);
void func_ov001_0230a58c(void);
void func_ov001_02314508(s32 a);
void func_ov001_02315de8(void);
void func_ov001_02313cb4(void);
void func_ov001_0230de20(void);
void func_ov001_0230fae0(void);
void func_ov001_02307398(void);
void func_ov001_0230b098(void);
void func_ov001_023147c0(s32 a, s32 b);
s32 func_ov001_0231398c(s32 a);
s32 func_ov001_0230f804(s32 a);
void func_0200412c(s32 a, s32 b, s32 c, s32 d);
void func_020367b4(s32 a, s32 b);

void func_ov001_0230c364(s32 arg) {
    vu32 z1;
    vu32 z2;
    vu32 fill1;
    vu32 fill2;
    u8 *blk;
    s32 i;

    blk = func_02034a40(data_0208fe80, 0x200, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill1 = 0xcccccccc;
        MIi_CpuClear32(fill1, blk, 0x200);
        *(u32 *)blk = 0x200;
        *(u32 *)(blk + 0x1fc) = 0x02467531;
        DC_StoreRange(blk, 0x200);
        blk += 0x20;
    }
    data_ov001_0231f5c0 = (UnkStruct_0230c364_c0 *)blk;
    MI_CpuFill8(blk, 0, 0x1d8);
    data_ov001_0231f5c0->unk4 = arg;
    data_ov001_0231f5c0->unk8 = data_022c4560.unk28;
    blk = func_02034a40(data_0208fe80, 0x60, 0x20);
    if ((u8 *)func_02006abc(0) != blk) {
        fill2 = 0xcccccccc;
        MIi_CpuClear32(fill2, blk, 0x60);
        *(u32 *)blk = 0x60;
        *(u32 *)(blk + 0x5c) = 0x02467531;
        DC_StoreRange(blk, 0x60);
        blk += 0x20;
    }
    data_ov001_0231f5c4 = (UnkStruct_0230c364_c4 *)blk;
    MI_CpuFill8(blk, 0, 0x30);
    *(vu32 *)0x4000000 &= ~0x1f00;
    *(vu32 *)0x4001000 &= ~0x1f00;
    func_0203a9d0(0);
    func_0203a6bc(0, 0x10, 0x40, 0, 0);
    func_0203a590(0);
    func_0203a504(0);
    func_02039ef8(0);
    func_02039d40(0);
    func_02002ec8();
    func_0203a43c(0);
    func_0203a288(0, 0x10, 0x40, 0, 0);
    func_0203a1f8(0);
    func_0203a18c(0);
    func_02002eb4();
    func_02002ea0();
    func_0203a6bc(0x40, 0x10, 0x40, 0, 0x200);
    func_02039d08(0);
    func_0203a9d0(0x10);
    func_02039ef8(7);
    func_02039b48();
    func_02039d40(0x20);
    func_020395f4();
    func_02002ab0(1, 0, 1);
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 4;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x108;
    func_0203a288(8, 0x100010, 0x40, 0, 0x400);
    func_02039d08(1);
    func_0203a43c(0x180);
    GXS_SetGraphicsMode(0);
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 4;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & 0x43) | 0x108;
    *(vu32 *)0x4000000 &= ~0x38000000;
    *(vu32 *)0x4000000 &= ~0x7000000;
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6000000, 0x1000);
    z2 = 0;
    MIi_CpuClear32(z2, (void *)0x6200000, 0x1000);
    *(vu16 *)0x4000304 &= ~0x8000;
    *(vu16 *)0x4000304 = (*(vu16 *)0x4000304 & ~0x20e) | 0x20e;
    GX_DispOn();
    *(vu32 *)0x4001000 |= 0x10000;
    data_0208922c.unk48 = 0x10;
    data_02087d04.unk48 = 0x10;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, 0x10);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, 0x10);
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 0;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x10;
    *(vu16 *)0x4000060 = *(vu16 *)0x4000060 & ~0x3002;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 4;
    *(vu16 *)0x4000340 = 0;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 8;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x20;
    func_02004040(0, 0, 0x7fff, 0, 1);
    *(vu32 *)0x4000540 = (data_0208f754.unk84 << 1) | data_0208f754.unk80;
    *(vu32 *)0x4000580 = 0xbfff0000;
    *(vu16 *)0x4000008 = *(vu16 *)0x4000008 & ~3;
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & ~3) | 1;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & ~3) | 2;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & ~3) | 3;
    *(vu16 *)0x4001008 = *(vu16 *)0x4001008 & ~3;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & ~3) | 1;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & ~3) | 2;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & ~3) | 3;
    func_02022270(0, 0x7fff);
    func_02022284(0, 0, -0x1000, 0);
    func_02022270(1, 0x4631);
    func_02022284(1, 0xec7, -0x600, 0);
    func_02022270(2, 0x2108);
    func_02022284(2, 0, 0x620, 0xec7);
    func_02004084(data_ov001_023171a0);
    func_ov001_0230be84(data_ov001_0231f5c4);
    func_ov001_0230bca8();
    func_ov001_0230b848();
    func_ov001_02314874();
    func_ov001_0230a58c();
    func_ov001_02314508(arg);
    func_ov001_02315de8();
    func_ov001_02313cb4();
    func_ov001_0230de20();
    func_ov001_0230fae0();
    func_ov001_02307398();
    func_ov001_0230b098();
    func_ov001_023147c0(data_ov001_0231f5c4->unk2C->unk8, data_ov001_0231f5c4->unk2C->unkC);
    data_ov001_0231f5c0->unk18 = func_ov001_0231398c(0);
    data_ov001_0231f5c0->unk1C = func_ov001_0230f804(0x80);
    func_0200412c(0, 0, 4, 0);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1500;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1400;
    func_020367b4(0, 0x1f);
    for (i = 0; i < 4; i++) {
        data_0208922c.unk0[i].a = 0;
        data_02087d04.unk0[i].a = 0;
        data_0208922c.unk0[i].b = 0;
        data_02087d04.unk0[i].b = 0;
    }
    data_0208922c.unk20 &= ~0xc0;
    data_02087d04.unk20 &= ~0xc0;
}
