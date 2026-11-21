#include <nitro/types.h>

void TransformDemo_ReleaseTransModels(void *a, s32 *p);
void TransformDemo_ReleasePlayerNarc(void *a, s32 *p);

void TransformDemo_ReleasePlayerModels(void *a, s32 *p) {
    if (*p != 2) {
        TransformDemo_ReleasePlayerNarc(a, p);
    } else {
        TransformDemo_ReleaseTransModels(a, p);
    }
}
