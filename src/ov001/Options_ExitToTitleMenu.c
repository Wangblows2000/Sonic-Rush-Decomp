#include <nitro/types.h>

void func_0205a4ec(void (*f)(void));
void TitleMenu_StartFromOptions(void);

#pragma thumb on
void Options_ExitToTitleMenu(void) {
    func_0205a4ec(TitleMenu_StartFromOptions);
}
#pragma thumb reset
