#include <nitro/types.h>

extern s32 gEffTrickSparkleTask;

void StageObj_Destructor(void *p);

void EffTrickSparkle_Destructor(void *p) {
    gEffTrickSparkleTask = 0;
    StageObj_Destructor(p);
}
