#include <nitro/types.h>

void File_Load(void *p, s32 v);

void SpecialStage_LoadHudFile(void *p) {
    File_Load(p, -1);
}
