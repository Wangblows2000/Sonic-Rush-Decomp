#include <nitro/types.h>

void SceneChange_ToSegaLogo(void);
void *SaveData_StartupOrShowError(void (*fn)(void));
BOOL func_0204ed94(void) { return SaveData_StartupOrShowError(SceneChange_ToSegaLogo) != NULL; }
