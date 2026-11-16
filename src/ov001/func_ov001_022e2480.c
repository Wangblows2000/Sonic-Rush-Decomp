#include <nitro/types.h>

void func_ov001_022e23b0(s32 a);
void func_ov001_022e22d0(s32 a);
void SaveData_StoreLives(void);
void SaveData_CommitOrShowError(void (*f)(void));

#pragma thumb on
void func_ov001_022e2480(s32 *p) {
    if (p[0] == 0) {
        func_ov001_022e23b0(p[1]);
    } else {
        func_ov001_022e22d0(p[2]);
    }
    SaveData_StoreLives();
    SaveData_CommitOrShowError(NULL);
}
#pragma thumb reset
