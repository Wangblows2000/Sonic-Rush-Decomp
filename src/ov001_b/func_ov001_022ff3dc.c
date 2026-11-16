#include <nitro/types.h>

typedef struct UnkStruct_022ff3dc_bl {
    s16 x;
    s16 y;
} UnkStruct_022ff3dc_bl;

typedef struct UnkStruct_022ff3dc_br {
    UnkStruct_022ff3dc_bl ofs[4];
    u8 pad10[0xc];
    u32 unk1C;
    u8 pad20[0x28];
    s16 unk48;
} UnkStruct_022ff3dc_br;

typedef struct UnkStruct_022ff3dc_f754 {
    u8 pad[0x80];
    u32 unk80;
    u32 unk84;
} UnkStruct_022ff3dc_f754;

typedef struct UnkStruct_022ff3dc_4960 {
    u8 pad[0x9c];
    u16 unk9C;
} UnkStruct_022ff3dc_4960;

typedef struct UnkStruct_022ff3dc_48bc {
    u8 pad[0x90];
    s16 v[9];
} UnkStruct_022ff3dc_48bc;

extern UnkStruct_022ff3dc_br gDisplayMain;
extern UnkStruct_022ff3dc_br gDisplaySub;
extern UnkStruct_022ff3dc_f754 data_0208f754;
extern UnkStruct_022ff3dc_4960 data_02074960;
extern UnkStruct_022ff3dc_48bc data_020748bc;

void func_02002b70(void);
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
void GX_DispOn(void);
void func_02004040(u32 a, u32 b, u16 c, u32 d, BOOL e);
void Vram_SetBankForBG(s32 a);
void Vram_SetBankForOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForTex(s32 a);
void Vram_SetBankForTexPltt(s32 a);
void func_02002ab0(s32 a, s32 b, s32 c);
void Vram_SetBankForBGExtPltt(s32 a);
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void GXS_SetGraphicsMode(u32 mode);
void Vram_SetBankForSubBGExtPltt(s32 a);
void ObjVram_Reset(s32 i);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void func_02022270(u32 index, u32 value);
void func_02022284(u32 idx, s32 x, s32 y, s32 z);

void func_ov001_022ff3dc(void) {
    u16 c;

    func_02002b70();
    *(vu32 *)0x4001000 &= ~0x10000;
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
    *(vu16 *)0x5000000 = 0xffff;
    *(vu16 *)0x5000400 = 0xffff;
    gDisplayMain.unk48 = gDisplaySub.unk48 = 0x10;
    *(vu32 *)0x4000000 &= ~0x1f00;
    *(vu32 *)0x4001000 &= ~0x1f00;
    *(vu16 *)0x4000304 |= 0x8000;
    *(vu16 *)0x4000304 = (*(vu16 *)0x4000304 & ~0x20e) | 0x20e;
    GX_DispOn();
    *(vu32 *)0x4001000 |= 0x10000;
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 0;
    *(vu32 *)0x4000540 = data_0208f754.unk80 | (data_0208f754.unk84 << 1);
    *(vu16 *)0x4000060 = *(vu16 *)0x4000060 & ~0x3002;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x10;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 4;
    *(vu16 *)0x4000340 = 0;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 8;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x20;
    func_02004040(0, 0, 0x7fff, 0, 0);
    *(vu32 *)0x4000580 = 0xbfff0000;
    Vram_SetBankForBG(0x10);
    Vram_SetBankForOBJ(0x40, 0x100010, 0x40, 0, 0x100);
    Vram_SetBankForTex(3);
    Vram_SetBankForTexPltt(0x20);
    func_02002ab0(1, 0, 1);
    Vram_SetBankForBGExtPltt(0);
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 4;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x10c;
    *(vu16 *)0x4000008 = *(vu16 *)0x4000008 & ~3;
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & ~3) | 1;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & ~3) | 2;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & ~3) | 3;
    Vram_SetBankForSubBG(0x80);
    Vram_SetBankForSubOBJ(0x100, 0x100010, 0x40, 0, 0x100);
    GXS_SetGraphicsMode(0);
    Vram_SetBankForSubBGExtPltt(0);
    *(vu16 *)0x4001008 = (*(vu16 *)0x4001008 & 0x43) | 4;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 0x4104;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & 0x43) | 0x404;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & 0x43) | 0x504;
    *(vu16 *)0x4001008 = *(vu16 *)0x4001008 & ~3;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & ~3) | 2;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & ~3) | 1;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & ~3) | 3;
    *(vu32 *)0x4000010 = 0;
    gDisplayMain.ofs[0].x = 0;
    gDisplayMain.ofs[0].y = 0;
    *(vu32 *)0x4000014 = 0;
    gDisplayMain.ofs[1].x = 0;
    gDisplayMain.ofs[1].y = 0;
    *(vu32 *)0x4000018 = 0;
    gDisplayMain.ofs[2].x = 0;
    gDisplayMain.ofs[2].y = 0;
    *(vu32 *)0x400001c = 0;
    gDisplayMain.ofs[3].x = 0;
    gDisplayMain.ofs[3].y = 0;
    *(vu32 *)0x4001010 = 0;
    gDisplaySub.ofs[0].x = 0;
    gDisplaySub.ofs[0].y = 0;
    *(vu32 *)0x4001014 = 0;
    gDisplaySub.ofs[1].x = 0;
    gDisplaySub.ofs[1].y = 0;
    *(vu32 *)0x4001018 = 0;
    gDisplaySub.ofs[2].x = 0;
    gDisplaySub.ofs[2].y = 0;
    *(vu32 *)0x400101c = 0;
    gDisplaySub.ofs[3].x = 0;
    gDisplaySub.ofs[3].y = 0;
    gDisplayMain.unk1C = 0;
    *(vu32 *)0x4000000 &= ~0xe000;
    gDisplaySub.unk1C = 0;
    *(vu32 *)0x4001000 &= ~0xe000;
    ObjVram_Reset(0);
    ObjVram_Reset(1);
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    c = data_02074960.unk9C;
    func_02022270(0, c);
    func_02022270(1, c);
    func_02022270(2, c);
    func_02022284(0, data_020748bc.v[0], data_020748bc.v[1], data_020748bc.v[2]);
    func_02022284(1, data_020748bc.v[3], data_020748bc.v[4], data_020748bc.v[5]);
    func_02022284(2, data_020748bc.v[6], data_020748bc.v[7], data_020748bc.v[8]);
}
