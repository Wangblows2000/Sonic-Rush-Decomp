#include <nitro/types.h>

u32 *NNS_SndArcGetSeqArcInfo(s32 idx);
u32 LoadSeqArc(u32 a, u32 b, u32 c);
u32 NNS_SndArcGetFileAddress(u32 idx);

s32 NNSi_SndArcLoadSeqArc(s32 idx, u32 flags, u32 b, u32 c, u32 *out) {
    u32 *info = NNS_SndArcGetSeqArcInfo(idx);
    u32 r;
    if (info == NULL) return 3;
    if (flags & 8) {
        r = LoadSeqArc(*info, b, c);
        if (r == 0) return 7;
    } else {
        r = NNS_SndArcGetFileAddress(*info);
    }
    if (out != NULL) *out = r;
    return 0;
}
