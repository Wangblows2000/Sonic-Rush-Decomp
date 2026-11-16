#include <nitro/types.h>

extern s32 data_02077964;
extern u32 data_0207d00c;
extern u32 data_0207d008;
void MI_WaitDma(s32 a);
void GX_SetBankForTexPltt(u32 a);
void GX_EndLoadTexPltt(void) {
    if (data_02077964 != -1) MI_WaitDma(data_02077964);
    GX_SetBankForTexPltt(data_0207d00c);
    data_0207d00c = 0;
    data_0207d008 = 0;
}
