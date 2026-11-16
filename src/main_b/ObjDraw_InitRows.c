#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_022c5220;
extern UnkStruct_022c5220 gObjDrawPaletteRows[16];
void ObjDraw_InitRows(void) { u8 i; for (i = 0; i < 16; i++) { gObjDrawPaletteRows[i].unk0 = 0; gObjDrawPaletteRows[i].unk2 = 0; } }
