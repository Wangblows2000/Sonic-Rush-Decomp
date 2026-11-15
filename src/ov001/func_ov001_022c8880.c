#include <nitro/types.h>

void func_020521b4(void);
void func_0205a4ec(void (*f)(void));
void func_02054e68(void);

#pragma thumb on
void func_ov001_022c8880(void) {
    func_020521b4();
    func_0205a4ec(func_02054e68);
}
#pragma thumb reset
