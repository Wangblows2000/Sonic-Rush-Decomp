#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; } UnkStruct_0201d010;
extern UnkStruct_0201d010 data_02083e10;
void NNSi_SndCaptureStop(void);

void NNS_SndCaptureStopEffect(void) {
    if (data_02083e10.unk0 == 0) return;
    if (data_02083e10.unk4 != 1) return;
    NNSi_SndCaptureStop();
}
