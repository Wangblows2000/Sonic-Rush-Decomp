#include <nitro/types.h>

typedef struct { u32 unk0; u16 unk4; u16 unk6; u8 unk8; u8 unk9; } UnkStruct_0201eb98;
UnkStruct_0201eb98 *NNS_SndArcGetSeqInfo(u32 a);
u32 StartSeq(void *a, u8 b, u16 c, u8 d, UnkStruct_0201eb98 *e, u32 f);

u32 NNS_SndArcPlayerStartSeq(void *a, u32 b) {
    UnkStruct_0201eb98 *r = NNS_SndArcGetSeqInfo(b);
    if (r == NULL) return 0;
    return StartSeq(a, r->unk9, r->unk4, r->unk8, r, b);
}
