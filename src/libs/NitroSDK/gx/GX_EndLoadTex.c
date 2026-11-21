/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_02077964[];
s32 MI_WaitDma();
extern u8 data_0207d010[];
s32 GX_SetBankForTex();
extern u8 data_0207d018[];
extern u8 data_0207d014[];
extern u8 data_0207d004[];

void GX_EndLoadTex(void) {
    s32 t0 = *(u32 *)data_02077964;
    if (t0 != -1) {
        MI_WaitDma(t0, -1);
    }
    GX_SetBankForTex(*(u32 *)data_0207d010);
    *(u32 *)data_0207d018 = 0;
    *(u32 *)data_0207d014 = 0;
    *(u32 *)data_0207d004 = 0;
    *(u32 *)data_0207d010 = 0;
}
