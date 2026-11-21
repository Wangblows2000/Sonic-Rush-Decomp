/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SaveData_IsEnabled();
s32 SaveData_Load();
s32 SaveMsg_StopSystems();
s32 SaveMsg_Request();

s32 SaveData_LoadOrShowError(s32 p0) {
    s32 t0 = SaveData_IsEnabled();
    if (t0 == 0) {
        return 1;
    }
    s32 t1 = SaveData_Load(t0);
    if (t1 == 0) {
        return 1;
    }
    SaveMsg_StopSystems(t1);
    if (t1 == 3) {
        SaveMsg_Request(3, p0);
    } else {
        SaveMsg_Request(1, 0);
    }
    return 0;
}
