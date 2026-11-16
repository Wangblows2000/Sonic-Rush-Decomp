#include <nitro/types.h>

void func_0205a4ec(void (*f)(void));
void Records_Start(void);

#pragma thumb on
void Options_ExitToRecords(void) {
    func_0205a4ec(Records_Start);
}
#pragma thumb reset
