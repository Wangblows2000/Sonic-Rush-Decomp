/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1980[];
s32 OS_Terminate();
s32 func_020405e0();
void func_0203e9cc();
s32 func_02011ac8();

s32 func_0203dbb4(void) {
    s32 t0 = *(u32 *)data_022c1980;
    s32 v1 = 1;
    if (t0 != 1) {
        s32 v2 = v1;
        if (t0 != 7) {
            v2 = 0;
        }
        v1 = v2;
    }
    if (v1 == 0) {
        OS_Terminate(t0, v1);
    }
    func_020405e0(3);
    s32 t3 = func_02011ac8((u32)func_0203e9cc);
    if (t3 == 2) {
        return 1;
    }
    func_020405e0(9);
    return 0;
}
