#include <nitro/types.h>

extern void *PMi_PreSleepCallbackList;
extern void *PMi_PostSleepCallbackList;

void PMi_ExecuteList(void *list);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
u32 OS_DisableIrqMask(u32 mask);
u32 OS_SetIrqMask(u32 mask);
BOOL CTRDG_IsExisting(void);
u32 PM_GetLCDPower(void);
void PM_GetBackLight(u32 *top, u32 *bottom);
void PM_SetBackLight(u32 lcd, u32 sw);
void GX_DispOff(void);
BOOL PMi_SendSleepStart(u16 a, u16 b);
void OS_Halt(void);
void OS_SpinWait(u32 cycles);
void PMi_SetLCDPower(u32 a, u32 b, u32 c, u32 d);
void GX_DispOn(void);
void PMi_SetLED(u32 a);
void PM_ForceToPowerOff(void);

#define REG_IME (*(vu16 *)0x04000208)
#define REG_IF (*(vu32 *)0x04000214)
#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DISPCNT_SUB (*(vu32 *)0x04001000)
#define VBLANK_COUNT (*(vu32 *)0x027ffc3c)

static inline u32 PM_GoSleepMode_GetVCount(void)
{
    return VBLANK_COUNT;
}

static inline u16 PM_GoSleepMode_DisableIrq(void)
{
    u16 prev = REG_IME;
    REG_IME = 0;
    return prev;
}

static inline u16 PM_GoSleepMode_EnableIrq(void)
{
    u16 prev = REG_IME;
    REG_IME = 1;
    return prev;
}

static inline u16 PM_GoSleepMode_RestoreIrq(u16 ime)
{
    u16 prev = REG_IME;
    REG_IME = ime;
    return prev;
}

void PM_GoSleepMode(u32 trigger, u32 logic, u16 keyPattern)
{
    BOOL pulledOut = FALSE;
    u32 top;
    u32 dispcnt;
    u32 dispcntSub;
    u32 enabled;
    u16 param;
    u32 prevMask;
    u16 prevIme;
    u32 bottom;
    u32 lcd;
    vu32 vcount;

    PMi_ExecuteList(PMi_PreSleepCallbackList);
    prevIme = PM_GoSleepMode_DisableIrq();
    enabled = OS_DisableInterrupts();
    prevMask = OS_DisableIrqMask(0x3fffff);
    OS_SetIrqMask(0x40000);
    OS_RestoreInterrupts(enabled);
    (void)PM_GoSleepMode_EnableIrq();

    if (trigger & 8) {
        if (*(u16 *)0x027ffc40 == 2) {
            trigger &= ~8;
        }
    }
    if (trigger & 0x10) {
        if (!CTRDG_IsExisting()) {
            trigger &= ~0x10;
        }
    }
    dispcnt = REG_DISPCNT;
    dispcntSub = REG_DISPCNT_SUB;
    lcd = PM_GetLCDPower();
    PM_GetBackLight(&top, &bottom);
    PM_SetBackLight(2, 0);

    vcount = PM_GoSleepMode_GetVCount();
    while (vcount == PM_GoSleepMode_GetVCount()) {
    }
    vcount = PM_GoSleepMode_GetVCount();
    GX_DispOff();
    REG_DISPCNT_SUB &= ~0x10000;
    while (vcount == PM_GoSleepMode_GetVCount()) {
    }
    vcount = PM_GoSleepMode_GetVCount();
    while (vcount == PM_GoSleepMode_GetVCount()) {
    }

    param = (u16)(trigger | (top << 5) | (bottom << 6));
    while (PMi_SendSleepStart(param, (u16)(logic | keyPattern)) != 0) {
    }
    OS_Halt();
    OS_SpinWait(0x332340);

    if (trigger & 8) {
        if (REG_IF & 0x100000) {
            pulledOut = TRUE;
        }
    }
    if (!pulledOut) {
        if (lcd == 1) {
            PMi_SetLCDPower(1, 1, 1, 0);
            GX_DispOn();
        } else {
            PMi_SetLED(1);
        }
        REG_DISPCNT = dispcnt;
        REG_DISPCNT_SUB = dispcntSub;
    }
    (void)OS_DisableInterrupts();
    OS_SetIrqMask(prevMask);
    OS_RestoreInterrupts(enabled);
    (void)PM_GoSleepMode_RestoreIrq(prevIme);
    if (pulledOut) {
        PM_ForceToPowerOff();
    }
    PMi_ExecuteList(PMi_PostSleepCallbackList);
}
