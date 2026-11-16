#include <nitro/types.h>

typedef struct {
    s16 key;
    s16 ref;
} UnkSlot_0206d2d0;

extern s8 data_022c521c;
extern s8 data_0207cfc4;
extern UnkSlot_0206d2d0 gObjDrawPaletteRows[];

void ObjDraw_LoadPaletteRow(void *res, u16 col, u16 idx, s32 screen);

u8 ObjDraw_AllocSpritePalette(void *res, u16 col, s16 key) {
    s8 i;

    if (!(key & 0xf000)) {
        for (i = data_022c521c; i < data_0207cfc4; i++) {
            if (key == gObjDrawPaletteRows[i].key) {
                gObjDrawPaletteRows[i].ref++;
                return i;
            }
        }
    }
    for (i = data_022c521c; i < data_0207cfc4; i++) {
        if (gObjDrawPaletteRows[i].ref == 0) {
            gObjDrawPaletteRows[i].ref++;
            break;
        }
    }
    if (i >= data_0207cfc4)
        return i - 1;
    gObjDrawPaletteRows[i].key = key;
    ObjDraw_LoadPaletteRow(res, col, (u8)i, 0);
    ObjDraw_LoadPaletteRow(res, col, (u8)i, 1);
    return i;
}
