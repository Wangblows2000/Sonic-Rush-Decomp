#include <nitro/types.h>

void File_Load(void *p, s32 v);

void SpecialStage_LoadStageDataFile(void *p) {
    File_Load(p, -1);
}
