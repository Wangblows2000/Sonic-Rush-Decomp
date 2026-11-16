/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SaveData_Format();
s32 MsgBox_RequestClose();
void func_02059e88();
extern u8 gTaskCurrent[];
s32 Task_Destroy();
s32 func_0205a304();
s32 func_0205a27c();

void func_02059eac(u8 *p0) {
    s32 t0 = SaveData_Format();
    if (t0 == 0) {
        MsgBox_RequestClose();
        *(u32 *)p0 = (u32)func_02059e88;
        return;
    }
    Task_Destroy(*(u32 *)gTaskCurrent);
    func_0205a304();
    func_0205a27c(2, 0);
}
