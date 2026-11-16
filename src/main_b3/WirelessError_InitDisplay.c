#include <nitro/types.h>

typedef struct { u8 pad0[8]; u16 unk8; } UnkStruct_02054b50;
extern UnkStruct_02054b50 data_0208926c;
extern UnkStruct_02054b50 data_02087d44;

void Vram_SetBankConfig(s32 a);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 mode);
void WirelessError_ResetBgAffine(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
static inline void Clear32_02054b50(void *dst, u32 size) {
    vu32 z = 0;
    MIi_CpuClear32(z, dst, size);
}

void WirelessError_InitDisplay(void) {
    Vram_SetBankConfig(0);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0x1000;
    *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | 0x1000;
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    *(vu16 *)0x04000304 = *(vu16 *)0x04000304 & ~0x8000;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & 0x43) | 0x84;
    data_0208926c.unk8 = 0;
    data_02087d44.unk8 = 0;
    WirelessError_ResetBgAffine();
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 0;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 1;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 2;
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & ~3) | 3;
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & ~3) | 0;
    *(vu16 *)0x0400100a = (*(vu16 *)0x0400100a & ~3) | 1;
    *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & ~3) | 2;
    *(vu16 *)0x0400100e = (*(vu16 *)0x0400100e & ~3) | 3;
    Clear32_02054b50((void *)0x06000000, 0x80000);
    Clear32_02054b50((void *)0x06200000, 0x20000);
    Clear32_02054b50((void *)0x05000000, 0x200);
    Clear32_02054b50((void *)0x05000400, 0x200);
    Clear32_02054b50((void *)0x05000200, 0x200);
    Clear32_02054b50((void *)0x05000600, 0x200);
}
#pragma thumb reset
