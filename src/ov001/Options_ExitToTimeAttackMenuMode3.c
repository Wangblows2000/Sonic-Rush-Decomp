#include <nitro/types.h>

void func_0205a4ec(void (*f)(void));
void SceneChange_ToTimeAttackMenuMode3(void);

#pragma thumb on
void Options_ExitToTimeAttackMenuMode3(void) {
    func_0205a4ec(SceneChange_ToTimeAttackMenuMode3);
}
#pragma thumb reset
