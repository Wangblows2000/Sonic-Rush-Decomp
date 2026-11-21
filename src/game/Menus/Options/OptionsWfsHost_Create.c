/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 OptionsWfsHost_Destroy();
extern u8 gOptionsWfsHostError[];
void OptionsWfsHost_State_WaitWfsReady();
void OptionsWfsHost_Destructor();
s32 Task_Create();
extern u8 gOptionsWfsHostTask[];
extern u8 data_022c44f8[];
s32 WirelessMgr_StartWfsParent();

void OptionsWfsHost_Create(s32 p0) {
    OptionsWfsHost_Destroy();
    *(u32 *)gOptionsWfsHostError = 0;
    s32 t0 = Task_Create((u32)OptionsWfsHost_State_WaitWfsReady, (u32)OptionsWfsHost_Destructor, 0, 0x1000, 0, 4);
    *(u32 *)gOptionsWfsHostTask = t0;
    *(u32 *)*(u32 *)((u8 *)t0 + 0x10) = 1;
    WirelessMgr_StartWfsParent(data_022c44f8, 1, 0x200, 0x10, p0, 8);
}

#pragma thumb reset
