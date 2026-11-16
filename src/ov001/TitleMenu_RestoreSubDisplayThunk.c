#include <nitro/types.h>

void TitleMenu_RestoreSubDisplay(void);

#pragma thumb on
void TitleMenu_RestoreSubDisplayThunk(void) {
    TitleMenu_RestoreSubDisplay();
}
#pragma thumb reset
