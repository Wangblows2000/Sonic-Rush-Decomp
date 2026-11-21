#include <nitro/types.h>

extern s32 data_ov000_02369f6c;

void StageObj_Destructor(void *p);

void MapObj13Part_Destructor_ov000_0230bfdc(void *p) {
    data_ov000_02369f6c = 0;
    StageObj_Destructor(p);
}
