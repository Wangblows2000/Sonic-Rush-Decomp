/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207fae4[];
extern u8 data_0207fae0[];
s32 CARD_TerminateForPulledOut();
s32 OS_Terminate();

void CARDi_PulledOutCallback(s32 p0, s32 p1) {
    if (((s32)p1 & 0x3f) == 0x11) {
        s32 t0 = *(u32 *)data_0207fae4;
        *(u32 *)data_0207fae0 = 1;
        s32 v1 = 1;
        if (t0 != 0) {
            s32 t2 = ((s32 (*)())t0)(v1, data_0207fae0, t0);
            v1 = t2;
        }
        if (v1 == 0) {
            return;
        }
        CARD_TerminateForPulledOut();
        return;
    }
    OS_Terminate();
}
