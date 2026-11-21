#include <nitro/types.h>

void ExtraBossObj_ReleaseFxModelRes(void);

void ExtraBossObj_ReleaseFxModelResThunk(void) {
    ExtraBossObj_ReleaseFxModelRes();
}
