#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct {
    vs32 unk0;
    vs32 unk4;
} UnkStruct_0208f7d4;

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_0235c6d8_Ofs;

typedef struct {
    UnkStruct_0235c6d8_Ofs ofs[4];
} UnkStruct_0235c6d8_Bg;

extern UnkStruct_0208f7d4 data_0208f7d4;
extern UnkStruct_0235c6d8_Bg gDisplayMain;
extern UnkStruct_0235c6d8_Bg gDisplaySub;

void func_02002f60(void);
void func_02002f4c(void);
void func_02002f28(void);
void func_02002f04(void);
void func_02002ef0(void);
void func_02002edc(void);
void func_02002ec8(void);
void func_02002e8c(void);
void func_02002e78(void);
void func_02002e50(void);
void func_02002e28(void);
void func_02002eb4(void);
void func_02002ea0(void);
void Vram_SetBankForBG(s32 a);
void Vram_SetBankForOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForBGExtPltt(s32 a);
void Vram_SetBankForOBJExtPltt(s32 a);
void Vram_SetBankForTex(s32 a);
void Vram_SetBankForTexPltt(s32 a);
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForSubBGExtPltt(s32 a);
void Vram_SetBankForSubOBJExtPltt(s32 a);
void func_02002ab0(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 mode);
void ObjVram_Reset(s32 a);
BOOL func_020513a8(void);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void func_02004040(s32 a, s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on

void func_ov000_0235c6d8(void) {
    vu32 z0;
    vu32 z1;

    REG32(0x4000000) = REG32(0x4000000) & ~0x1f00;
    REG32(0x4001000) = REG32(0x4001000) & ~0x1f00;
    func_02002f60();
    func_02002f4c();
    func_02002f28();
    func_02002f04();
    func_02002ef0();
    func_02002edc();
    func_02002ec8();
    func_02002e8c();
    func_02002e78();
    func_02002e50();
    func_02002e28();
    func_02002eb4();
    func_02002ea0();
    Vram_SetBankForBG(0);
    Vram_SetBankForOBJ(0, 0x10, 0x40, 0, 0);
    Vram_SetBankForBGExtPltt(0);
    Vram_SetBankForOBJExtPltt(0);
    Vram_SetBankForTex(0);
    Vram_SetBankForTexPltt(0);
    func_02002ec8();
    Vram_SetBankForSubBG(0);
    Vram_SetBankForSubOBJ(0, 0x10, 0x40, 0, 0);
    Vram_SetBankForSubBGExtPltt(0);
    Vram_SetBankForSubOBJExtPltt(0);
    func_02002eb4();
    func_02002ea0();
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    REG16(0x4000304) |= 0x8000;
    func_02002ab0(1, 0, 1);
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    GXS_SetGraphicsMode(0);
    Vram_SetBankForOBJ(0x10, 0x100010, 0x40, 0, 0x400);
    Vram_SetBankForOBJExtPltt(0);
    ObjVram_Reset(0);
    Vram_SetBankForBG(2);
    Vram_SetBankForBGExtPltt(0x40);
    Vram_SetBankForSubOBJ(8, 0x200010, 0x40, 0, 0x400);
    Vram_SetBankForSubOBJExtPltt(0);
    ObjVram_Reset(1);
    Vram_SetBankForSubBG(4);
    Vram_SetBankForSubBGExtPltt(0x80);
    Vram_SetBankForTex(1);
    Vram_SetBankForTexPltt(0x20);
    if (!func_020513a8()) {
        Vram_ResetTexAndPltt();
        VramPltt_Reset();
    }
    data_0208f7d4.unk0 = 0;
    data_0208f7d4.unk4 = 0;
    REG16(0x4000060) &= ~0x3002;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x10;
    REG16(0x4000060) &= (u16)~0x3004;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x8;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x20;
    func_02004040(0, 0, 0x7fff, 0, 0);
    {
        s32 zw = data_0208f7d4.unk4;
        s32 am = data_0208f7d4.unk0;
        REG32(0x4000540) = am | (zw << 1);
    }
    REG32(0x4000580) = 0xbfff0000;
    REG16(0x400000c) = (REG16(0x400000c) & 0x43) | 0x84;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x188;
    REG16(0x4001008) = (REG16(0x4001008) & 0x43) | 0x84;
    REG16(0x400100c) = (REG16(0x400100c) & 0x43) | 0x188;
    REG16(0x400100e) = (REG16(0x400100e) & 0x43) | 0x38c;
    REG16(0x4000008) = (REG16(0x4000008) & ~3);
    REG16(0x400000a) = (REG16(0x400000a) & ~3) | 1;
    REG16(0x400000c) = (REG16(0x400000c) & ~3) | 2;
    REG16(0x400000e) = (REG16(0x400000e) & ~3) | 3;
    REG16(0x4001008) = (REG16(0x4001008) & ~3);
    REG16(0x400100a) = (REG16(0x400100a) & ~3) | 1;
    REG16(0x400100c) = (REG16(0x400100c) & ~3) | 2;
    REG16(0x400100e) = (REG16(0x400100e) & ~3) | 3;
    gDisplayMain.ofs[0].x = 0;
    gDisplayMain.ofs[0].y = 0;
    gDisplayMain.ofs[1].x = 0;
    gDisplayMain.ofs[1].y = 0;
    gDisplayMain.ofs[2].x = 0;
    gDisplayMain.ofs[2].y = 0;
    gDisplayMain.ofs[3].x = 0;
    gDisplayMain.ofs[3].y = 0;
    gDisplaySub.ofs[0].x = 0;
    gDisplaySub.ofs[0].y = 0;
    gDisplaySub.ofs[1].x = 0;
    gDisplaySub.ofs[1].y = 0;
    gDisplaySub.ofs[2].x = 0;
    gDisplaySub.ofs[2].y = 0;
    gDisplaySub.ofs[3].x = 0;
    gDisplaySub.ofs[3].y = 0;
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000000, 0x10000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200000, 0x10000);
    REG32(0x4000000) = (REG32(0x4000000) & ~0x1f00) | 0x1d00;
    REG32(0x4001000) = (REG32(0x4001000) & ~0x1f00) | 0x1800;
}
#pragma thumb reset
