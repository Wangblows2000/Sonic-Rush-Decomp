#include <nitro/types.h>

typedef struct {
    s16 v;
    s16 pad;
} UnkPair_0206d3fc;

extern s8 data_022c521c;
extern s8 data_0207cfc4;
extern UnkPair_0206d3fc data_022c5222[];
extern UnkPair_0206d3fc gObjDrawPaletteRows[];

void ObjDraw_ReleaseSpritePalette(s32 i) {
    if (i < data_022c521c)
        return;
    if (i >= data_0207cfc4)
        return;
    if (data_022c5222[i].v != 0)
        data_022c5222[i].v--;
    if (data_022c5222[i].v == 0)
        gObjDrawPaletteRows[i].v = 0;
}
