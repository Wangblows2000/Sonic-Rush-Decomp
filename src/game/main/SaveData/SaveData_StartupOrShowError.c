#include <nitro/types.h>

s32 SaveData_Startup(void);
void SaveMsg_Request(u32 a, u32 b);
void SaveMsg_StopSystems(void);

s32 SaveData_StartupOrShowError(u32 arg) {
    switch (SaveData_Startup()) {
    case 0:
    case 4:
        return 1;
    case 3:
        SaveMsg_Request(3, arg);
        return 0;
    case 6:
    case 7:
        return 1;
    case 1:
    case 2:
    case 5:
    default:
        SaveMsg_StopSystems();
        SaveMsg_Request(1, 0);
        return 0;
    }
}
