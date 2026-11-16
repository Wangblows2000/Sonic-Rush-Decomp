#include <nitro/types.h>

void *WBT_Unk_02015640(void);
void WBT_Unk_020138b4(void *a, u32 b, u32 c, s32 d);
void WBT_Unk_02014f5c(u32 b, u32 c, s32 d) { if (d <= 0) return; WBT_Unk_020138b4(WBT_Unk_02015640(), b, c, d); }
