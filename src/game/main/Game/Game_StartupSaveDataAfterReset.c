#include <nitro/types.h>

void SceneChange_ToSegaLogo(void);
void *SaveData_StartupOrShowError(void (*fn)(void));
BOOL Game_StartupSaveDataAfterReset(void) { return SaveData_StartupOrShowError(SceneChange_ToSegaLogo) != NULL; }
