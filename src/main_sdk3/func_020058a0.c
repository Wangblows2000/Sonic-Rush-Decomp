#include <nitro/types.h>

typedef struct {
    vu32 lockFlag;
    vu16 ownerID;
    vu16 extension;
} UnkOSLockWord_020058a0;

extern BOOL data_0207d07c;

s32 func_02005838(u16 lockID, UnkOSLockWord_020058a0 *lockp, void (*ctrlFuncp)(void));
s32 func_02005798(u16 lockID, UnkOSLockWord_020058a0 *lockp, void (*ctrlFuncp)(void));
void func_02005628(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#define REG_EXMEMCNT (*(vu16 *)0x04000204)

void func_020058a0(void)
{
    UnkOSLockWord_020058a0 *lockp = (UnkOSLockWord_020058a0 *)0x027ffff0;

    if (data_0207d07c) {
        return;
    }
    data_0207d07c = TRUE;
    lockp->lockFlag = 0;
    (void)func_02005838(0x7e, lockp, NULL);
    while (lockp->extension != 0) {
        func_02005628();
    }
    ((u32 *)0x027fffb0)[0] = 0xffffffff;
    ((u32 *)0x027fffb0)[1] = 0xffff0000;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, (void *)0x027fffc0, 0x28);
    }
    REG_EXMEMCNT |= 0x800;
    REG_EXMEMCNT |= 0x80;
    (void)func_02005798(0x7e, lockp, NULL);
    (void)func_02005838(0x7f, lockp, NULL);
}
