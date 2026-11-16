#include <nitro/types.h>

void TransformDemo_DrawTransModels(void *a, s32 *p);
void TransformDemo_DrawPlayerModel(void *a, s32 *p);

void TransformDemo_DrawPlayerModels(void *a, s32 *p) {
    if (*p != 2) {
        TransformDemo_DrawPlayerModel(a, p);
    } else {
        TransformDemo_DrawTransModels(a, p);
    }
}
