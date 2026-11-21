#include <nitro/types.h>

void ExtraBossObj_DrawModel(void *p);
void ExtraBossObj_DrawSprite(void *p);

void ExtraBossObj_Draw_Default(u8 *p) {
    if (*(void **)(p + 0x94) != NULL) {
        ExtraBossObj_DrawModel(p);
    } else if (*(void **)(p + 0x84) != NULL) {
        ExtraBossObj_DrawModel(p);
    } else if (*(void **)(p + 0x90) != NULL) {
        ExtraBossObj_DrawSprite(p);
    }
}
