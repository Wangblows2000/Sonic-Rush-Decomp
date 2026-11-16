#include <nitro/types.h>

typedef struct { u8 unk0[5]; u8 unk5; u8 unk6; } UnkInfo_02020278;
UnkInfo_02020278 *NNS_SndArcGetStrmInfo(u32 idx);
BOOL func_0201fcc4(void *s, UnkInfo_02020278 *info, u8 a, u8 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h);

BOOL func_02020278(void *s, u32 idx, u32 arg) {
    UnkInfo_02020278 *info = NNS_SndArcGetStrmInfo(idx);
    if (info == NULL) return FALSE;
    return func_0201fcc4(s, info, info->unk6, info->unk5, idx, arg, 0, 0, 0, 0);
}
