#include <nitro/types.h>

extern u32 gZoneSoundData;
void Sound_ResetHeap(void);
void Stage_FreeZoneSoundData(void) { Sound_ResetHeap(); gZoneSoundData = 0; }
