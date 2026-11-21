#include <nitro/types.h>

void SaveData_CommitOrShowError(void (*f)(void));
void Records_Start(void);

#pragma thumb on
void Options_ExitToRecords(void) {
    SaveData_CommitOrShowError(Records_Start);
}
#pragma thumb reset
