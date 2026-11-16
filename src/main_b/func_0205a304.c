#include <nitro/types.h>

extern u32 gSystemFlags;
void PaletteTransfer_Clear(void);
void ScreenTransfer_Clear(void);
void VRAMTransfer_Clear(void);
void func_0201b348(void);
void func_0205a304(void) { PaletteTransfer_Clear(); ScreenTransfer_Clear(); VRAMTransfer_Clear(); gSystemFlags &= ~0xc00; func_0201b348(); }
