#include <nitro/types.h>

void NNS_SndStopSoundAll(void);
void Sound_ResetHeap(void);
void GameOverSeq_Destructor(void) { NNS_SndStopSoundAll(); Sound_ResetHeap(); }
