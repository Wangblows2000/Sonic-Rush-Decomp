#include <nitro/types.h>

void func_0205a4ec(void (*f)(void));
void SceneChange_ToTimeAttackMenuMode2(void);

#pragma thumb on
void Options_ExitToTimeAttackMenuMode2(void) {
    func_0205a4ec(SceneChange_ToTimeAttackMenuMode2);
}
#pragma thumb reset
