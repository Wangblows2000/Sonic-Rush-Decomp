#include <nitro/types.h>

void func_020072bc(void);
void func_02007584(void);

#pragma thumb on
void func_0205fd50(void) {
    func_020072bc();
    func_02007584();
}
#pragma thumb reset
