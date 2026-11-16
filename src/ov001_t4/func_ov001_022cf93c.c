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
} UnkStruct_022cf93c_Ofs;

typedef struct {
    UnkStruct_022cf93c_Ofs ofs[4];
} UnkStruct_022cf93c_Bg;

typedef struct {
    u8 pad[8];
    s16 brightness;
} UnkStruct_022cf93c_Br;

typedef struct {
    u32 unk[4];
} UnkStruct_022cf93c_Mtx;

extern UnkStruct_0208f7d4 data_0208f7d4;
extern UnkStruct_022cf93c_Bg data_0208922c;
extern UnkStruct_022cf93c_Bg data_02087d04;
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];
extern UnkStruct_022cf93c_Br data_0208926c;
extern UnkStruct_022cf93c_Br data_02087d44;

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
void func_0203a9d0(s32 a);
void func_0203a6bc(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a590(s32 a);
void func_0203a504(s32 a);
void func_02039ef8(s32 a);
void func_02039d40(s32 a);
void func_0203a43c(s32 a);
void func_0203a288(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a1f8(s32 a);
void func_0203a18c(s32 a);
void func_02002ab0(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 mode);
void func_02039d08(s32 a);
void func_02039b48(void);
void func_020395f4(void);
void func_02004040(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203ac70(s32 a);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02000cb4(UnkStruct_022cf93c_Mtx *m);
void func_02003d94(void *a, UnkStruct_022cf93c_Mtx *m, s32 c, s32 d, s32 e, s32 f);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

#pragma thumb on

void func_ov001_022cf93c(void) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;

    func_0203ac70(0);
    func_0203a590(0);
    func_0203a1f8(0);
    func_02039b48();
    func_020395f4();
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    REG16(0x4000304) |= 0x8000;
    func_02002ab0(1, 0, 0);
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 4;
    GXS_SetGraphicsMode(0);
    REG16(0x400100e) = (REG16(0x400100e) & 0x43) | 4;
    if (data_0208926c.brightness == 0x10) {
        data_0208926c.brightness = 0x10;
        data_02087d44.brightness = 0x10;
    } else {
        data_0208926c.brightness = -16;
        data_02087d44.brightness = -16;
    }
    data_0208922c.ofs[0].x = 0;
    data_0208922c.ofs[0].y = 0;
    data_0208922c.ofs[1].x = 0;
    data_0208922c.ofs[1].y = 0;
    data_0208922c.ofs[2].x = 0;
    data_0208922c.ofs[2].y = 0;
    data_0208922c.ofs[3].x = 0;
    data_0208922c.ofs[3].y = 0;
    data_02087d04.ofs[0].x = 0;
    data_02087d04.ofs[0].y = 0;
    data_02087d04.ofs[1].x = 0;
    data_02087d04.ofs[1].y = 0;
    data_02087d04.ofs[2].x = 0;
    data_02087d04.ofs[2].y = 0;
    data_02087d04.ofs[3].x = 0;
    data_02087d04.ofs[3].y = 0;
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000000, 0x80000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200000, 0x20000);
    z2 = 0;
    MIi_CpuClear32(z2, (void *)0x5000000, 0x200);
    z3 = 0;
    MIi_CpuClear32(z3, (void *)0x5000400, 0x200);
    z4 = 0;
    MIi_CpuClear32(z4, (void *)0x5000200, 0x200);
    z5 = 0;
    MIi_CpuClear32(z5, (void *)0x5000600, 0x200);
    REG32(0x4000000) = (REG32(0x4000000) & ~0x1f00) | 0x800;
    REG32(0x4001000) = (REG32(0x4001000) & ~0x1f00) | 0x800;
}
#pragma thumb reset
