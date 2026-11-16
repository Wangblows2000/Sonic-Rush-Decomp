/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SaveData_IsEnabled();
s32 SaveData_Commit();
s32 SaveMsg_StopSystems();
s32 SaveMsg_Request();

s32 SaveData_CommitOrShowError(s32 p0) {
    s32 t0 = SaveData_IsEnabled();
    if (t0 == 0) {
        if ((s32)p0 != 0) {
            ((s32 (*)())(s32)p0)();
        }
        return 1;
    }
    s32 t1 = SaveData_Commit(t0);
    if (t1 != 0) {
        SaveMsg_StopSystems();
        SaveMsg_Request(2, 0);
        return 0;
    }
    if ((s32)p0 != 0) {
        ((s32 (*)())(s32)p0)(t1);
    }
    return 1;
}
