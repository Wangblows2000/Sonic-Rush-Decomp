#include <nitro/types.h>

extern u32 gSystemFlags;
void PaletteTransfer_Clear(void);
void ScreenTransfer_Clear(void);
void VRAMTransfer_Clear(void);
void NNS_SndStopSoundAll(void);
void SaveMsg_StopSystems(void) { PaletteTransfer_Clear(); ScreenTransfer_Clear(); VRAMTransfer_Clear(); gSystemFlags &= ~0xc00; NNS_SndStopSoundAll(); }
