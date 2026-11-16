#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
} UnkDisp_0204edc0;

extern u32 data_02087cc0;
extern UnkDisp_0204edc0 data_0208922c;

void func_0203ac70(s32 a);
void func_02002ab0(s32 a, s32 b, s32 c);
void GX_DispOn(void);
BOOL func_0204ed94(void);
BOOL func_0204ed68(void);
s32 func_0205a37c(void (*fn)(void));
void func_02054e4c(void);
void func_02054e68(void);

#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DB_DISPCNT (*(vu32 *)0x04001000)
#define REG_POWCNT (*(vu16 *)0x04000304)

void func_0204edc0(void) {
    func_0203ac70(0);
    func_02002ab0(1, 0, 0);
    REG_DISPCNT &= ~0x1f00;
    REG_POWCNT = (REG_POWCNT & ~0x20e) | 0x202;
    REG_POWCNT |= 0x8000;
    GX_DispOn();
    REG_DB_DISPCNT |= 0x10000;
    data_0208922c.unk4 = 0;
    data_0208922c.unk6 = 0;
    if ((data_02087cc0 & 0x80000000) == 0) {
        if (!func_0204ed94())
            return;
    } else {
        if (!func_0204ed68())
            return;
    }
    if (!func_0205a37c(func_02054e4c))
        return;
    if ((data_02087cc0 & 0x80000000) == 0)
        func_02054e4c();
    else
        func_02054e68();
}
