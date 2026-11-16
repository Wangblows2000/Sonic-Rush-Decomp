#include <nitro/types.h>

extern u32 data_02077964;
extern u8 gGameLanguage[];

void func_02006834(void);
void func_02005e64(void);
void func_02006f80(void);
void func_0200701c(void);
void func_0200f160(void);
void func_02002a64(void);
void func_02002c10(void);
void func_02004398(void);
void func_02025a60(void);
void func_020224bc(void);
void func_02005520(u32 mask, void *func);
void func_01ff861c(void);
u32 OS_EnableIrqMask(u32 mask);
void GX_VBlankIntr(BOOL enable);
s32 OS_EnableInterrupts(void);
void GX_SetBankForLCDC(u32 bank);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);
void func_02002ea0(void);
void System_InitState(void);
void Heap_InitAll(void);
void FileRequest_Init(void);
void func_020392ec(void);
void func_020384ac(void);
void Pad_Init(void);
void Task_Init(void);
void Vram_Init(void);
void PaletteTransfer_Init(void);
void VRAMTransfer_Init(void);
void ScreenTransfer_Init(void);
void TransferJob_InitPool(void);
void Oam_Init(void);
void func_020336c8(void);
void func_02031cc0(void);
void Sound_Init(void);
void Rand_SeedFromRTC(void);
void Wireless_Init(void);
void Touch_Init(void);
void Mic_Init(void);
void func_0202ff78(void);
void func_020105e0(void (*func)(void));
void func_02007828(void *p);
void func_0200e378(u32 a);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02004040(u32 color, u32 alpha, u32 depth, u32 polygonID, BOOL fog);
void GX_DispOn(void);

#define REG_IME (*(vu16 *)0x04000208)
#define REG_DISP3DCNT (*(vu16 *)0x04000060)
#define REG_POWCNT (*(vu16 *)0x04000304)

static inline u16 EnableIrq_02030598(void) {
    u16 prep = REG_IME;
    REG_IME = 1;
    return prep;
}

void System_Init(void) {
    vu16 zh;
    vu32 z2;
    vu32 z1;
    vu32 z3;
    func_02006834();
    func_02005e64();
    func_02006f80();
    func_0200701c();
    func_0200f160();
    func_02002a64();
    data_02077964 = 3;
    func_02002c10();
    func_02004398();
    func_02025a60();
    func_020224bc();
    EnableIrq_02030598();
    func_02005520(1, func_01ff861c);
    OS_EnableIrqMask(1);
    GX_VBlankIntr(TRUE);
    OS_EnableInterrupts();
    GX_SetBankForLCDC(0x1FF);
    z1 = 0;
    MIi_CpuClearFast(z1, (void *)0x06800000, 0xA4000);
    func_02002ea0();
    z2 = 0xC0;
    MIi_CpuClearFast(z2, (void *)0x07000000, 0x400);
    z3 = 0;
    MIi_CpuClearFast(z3, (void *)0x05000000, 0x400);
    System_InitState();
    Heap_InitAll();
    FileRequest_Init();
    func_020392ec();
    func_020384ac();
    Pad_Init();
    Task_Init();
    Vram_Init();
    PaletteTransfer_Init();
    VRAMTransfer_Init();
    ScreenTransfer_Init();
    TransferJob_InitPool();
    Oam_Init();
    func_020336c8();
    func_02031cc0();
    Sound_Init();
    Rand_SeedFromRTC();
    Wireless_Init();
    Touch_Init();
    Mic_Init();
    func_020105e0(func_0202ff78);
    func_02007828(gGameLanguage);
    func_0200e378(0);
    REG_DISP3DCNT &= ~0x3002;
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 0x10;
    REG_DISP3DCNT &= 0xCFFB;
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 8;
    REG_DISP3DCNT &= 0xCFDF;
    zh = 0;
    MIi_CpuClear16(zh, (void *)0x04000330, 0x10);
    func_02004040(0, 0, 0x7FFF, 0, FALSE);
    *(vu32 *)0x04000580 = 0xBFFF0000;
    REG_POWCNT = (REG_POWCNT & ~0x20E) | 0x202;
    REG_POWCNT |= 0x8000;
    GX_DispOn();
    *(vu32 *)0x04001000 |= 0x10000;
}
