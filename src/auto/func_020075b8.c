/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207d388[];
extern u8 data_0207d394[];
s32 func_02005348();
extern u8 data_0207d390[];
extern u8 data_0207d38c[];

void func_020075b8(void) {
    if (*(u16 *)data_0207d388 != 0) {
        return;
    }
    *(u16 *)data_0207d388 = 1;
    *(u32 *)data_0207d394 = 0;
    *(u32 *)((u8 *)data_0207d394 + 0x4) = 0;
    func_02005348(4, data_0207d394, data_0207d388, 0);
    *(u32 *)data_0207d390 = 0;
    *(u32 *)data_0207d38c = 0;
}
