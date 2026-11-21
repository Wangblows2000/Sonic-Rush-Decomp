#include <nitro/types.h>

extern u16 gPaletteAnimRefCount[];
void PaletteAnim_Release(s32 i) { if (gPaletteAnimRefCount[i] != 0) gPaletteAnimRefCount[i]--; }
