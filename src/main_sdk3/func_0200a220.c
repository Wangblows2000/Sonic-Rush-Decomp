#include <nitro/types.h>

typedef void (*UnkIrqFunc)(void);

typedef struct {
    u8 pad0[0x388];
    u32 field_388;
} UnkSysWork_0200a220;

extern u16 data_0207ef24;
extern u32 data_0207ef28[32];
extern void func_02009fb4(void);
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern u32 OS_ResetRequestIrqMask(u32 mask);
extern u32 OS_EnableIrqMask(u32 mask);
extern void func_02005520(u32 intrBit, UnkIrqFunc function);

#define REG_PXI_SYNC (*(vu16 *)0x04000180)
#define REG_PXI_FIFO_CNT (*(vu16 *)0x04000184)

void func_0200a220(void)
{
    u32 state = OS_DisableInterrupts();
    if (data_0207ef24 == 0) {
        UnkSysWork_0200a220 *w;
        s32 i;
        s32 timeout;
        s32 local;
        s32 count;
        data_0207ef24 = 1;
        w = (UnkSysWork_0200a220 *)0x027ffc00;
        w->field_388 = 0;
        for (i = 0; i < 32; i++) {
            data_0207ef28[i] = 0;
        }
        REG_PXI_FIFO_CNT = 0xc408;
        OS_ResetRequestIrqMask(0x40000);
        func_02005520(0x40000, func_02009fb4);
        OS_EnableIrqMask(0x40000);
        for (count = 0;; count++) {
            local = REG_PXI_SYNC & 0xf;
            REG_PXI_SYNC = (u16)(local << 8);
            if (local == 0 && count > 4) {
                break;
            }
            for (timeout = 1000; (REG_PXI_SYNC & 0xf) == local; timeout--) {
                if (timeout <= 0) {
                    count = 0;
                    break;
                }
            }
        }
    }
    OS_RestoreInterrupts(state);
}
