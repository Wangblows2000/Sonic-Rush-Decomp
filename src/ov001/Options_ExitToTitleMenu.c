#include <nitro/types.h>

void SaveData_CommitOrShowError(void (*f)(void));
void TitleMenu_StartFromOptions(void);

#pragma thumb on
void Options_ExitToTitleMenu(void) {
    SaveData_CommitOrShowError(TitleMenu_StartFromOptions);
}
#pragma thumb reset
