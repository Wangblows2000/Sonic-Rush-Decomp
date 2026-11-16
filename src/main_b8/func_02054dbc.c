#include <nitro/types.h>
#include <nitro/fs_overlay.h>

extern s32 data_0224ff40;

void func_020521b4(void);
void func_0201b348(void);
void func_02036894(void);
void func_0204331c(s32 mode);
void func_02054a04(void);
void func_02036928(void);
void func_0203051c(u32 overlayId, void (*fn)(void));
void func_02054d70(void);

#pragma thumb on
void func_02054dbc(void) {
    func_020521b4();
    func_0201b348();
    func_02036894();
    data_0224ff40 = 0;
    func_0204331c(0);
    func_02054a04();
    func_02036928();
    func_0203051c(FS_OVERLAY_ID(ov001), func_02054d70);
}
#pragma thumb reset
