#include <nitro/types.h>

void TransformDemo_Unk_ov001_022de774(void *a, s32 *p);
void TransformDemo_Unk_ov001_022dedf4(void *a, s32 *p);

void TransformDemo_UpdatePlayerModels(void *a, s32 *p) {
    if (*p != 2) {
        TransformDemo_Unk_ov001_022dedf4(a, p);
    } else {
        TransformDemo_Unk_ov001_022de774(a, p);
    }
}
