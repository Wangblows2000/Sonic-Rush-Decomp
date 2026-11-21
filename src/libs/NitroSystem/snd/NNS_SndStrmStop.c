#include <nitro/types.h>

void ForceStopStrm_stream(void *p);

typedef struct { u8 unk0[0x24]; s32 unk24_0 : 1; } UnkStruct_0201c458;

void NNS_SndStrmStop(UnkStruct_0201c458 *s) {
    if (!s->unk24_0) return;
    ForceStopStrm_stream(s);
}
