#include <nitro/types.h>

void TransformDemo_LoadTransModels(void *a, s32 *p);
void TransformDemo_LoadPlayerNarc(void *a, s32 *p);

void func_ov001_022df440(void *a, s32 *p) {
    if (*p != 2) {
        TransformDemo_LoadPlayerNarc(a, p);
    } else {
        TransformDemo_LoadTransModels(a, p);
    }
}
