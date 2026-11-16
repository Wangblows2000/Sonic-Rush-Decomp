#include <nitro/types.h>

#pragma thumb on

void ClearDemo_GoToNextScene(void);
void ClearDemo_Finish(void) { ClearDemo_GoToNextScene(); }

#pragma thumb reset
