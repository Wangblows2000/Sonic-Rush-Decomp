/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207d388[];
extern u8 data_0207d394[];
s32 OS_DisableIrqMask();
extern u8 data_0207d390[];
extern u8 data_0207d38c[];

void OS_InitVAlarm(void) {
    if (*(u16 *)data_0207d388 != 0) {
        return;
    }
    *(u16 *)data_0207d388 = 1;
    *(u32 *)data_0207d394 = 0;
    *(u32 *)((u8 *)data_0207d394 + 0x4) = 0;
    OS_DisableIrqMask(4, data_0207d394, data_0207d388, 0);
    *(u32 *)data_0207d390 = 0;
    *(u32 *)data_0207d38c = 0;
}
