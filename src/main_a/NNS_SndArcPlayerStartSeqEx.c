#include <nitro/types.h>

typedef struct { u32 unk0; u16 unk4; u16 unk6; u8 unk8; u8 unk9; } UnkStruct_0201eb20;
UnkStruct_0201eb20 *NNS_SndArcGetSeqInfo(u32 a);
u32 StartSeq(void *a, s32 b, s32 c, s32 d, UnkStruct_0201eb20 *e, u32 f);

u32 NNS_SndArcPlayerStartSeqEx(void *a, s32 b, s32 c, s32 d, u32 idx) {
    UnkStruct_0201eb20 *r = NNS_SndArcGetSeqInfo(idx);
    if (r == NULL) return 0;
    if (d < 0) d = r->unk8;
    if (c < 0) c = r->unk4;
    if (b < 0) b = r->unk9;
    return StartSeq(a, b, c, d, r, idx);
}
