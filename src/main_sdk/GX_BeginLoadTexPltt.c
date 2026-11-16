#include <nitro/types.h>

s32 GX_ResetBankForTexPltt(void);
extern s32 data_0207d00c;
extern u16 data_02072570[];
extern s32 data_0207d008;
void GX_BeginLoadTexPltt(void) { s32 v = GX_ResetBankForTexPltt(); data_0207d00c = v; data_0207d008 = data_02072570[v >> 4] << 12; }
