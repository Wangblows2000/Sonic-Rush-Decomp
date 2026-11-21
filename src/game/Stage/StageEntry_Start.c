#include <nitro/types.h>

void Stage_ResetProgress(void);
void StageStart_Create(s32 a);
void StageEntry_Start(void) { Stage_ResetProgress(); StageStart_Create(3); }
