#include <nitro/types.h>

typedef struct { u32 unk0; u16 unk4; u16 unk6; u8 unk8; u8 unk9; } UnkEntry_0201ea88;
u32 *NNS_SndArcGetSeqArcInfo(s32 idx);
void *func_0201d0f8(u32 idx);
UnkEntry_0201ea88 *NNSi_SndSeqArcGetSeqInfo(void *p, s32 idx);
u32 StartSeqArc(void *a, u8 b, u16 c, u8 d, UnkEntry_0201ea88 *e, void *f, s32 g, s32 h);

u32 NNS_SndArcPlayerStartSeqArc(void *a, s32 idx, s32 sub) {
    u32 *info;
    void *p;
    UnkEntry_0201ea88 *e;
    info = NNS_SndArcGetSeqArcInfo(idx);
    if (info == NULL) return 0;
    p = func_0201d0f8(*info);
    if (p == NULL) return 0;
    e = NNSi_SndSeqArcGetSeqInfo(p, sub);
    if (e == NULL) return 0;
    return StartSeqArc(a, e->unk9, e->unk4, e->unk8, e, p, idx, sub);
}
