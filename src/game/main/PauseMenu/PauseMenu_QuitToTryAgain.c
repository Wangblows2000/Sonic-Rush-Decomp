#include <nitro/types.h>

void SceneChange_ToTryAgain(void);
void PauseMenu_QuitToTryAgain(void) { SceneChange_ToTryAgain(); }
