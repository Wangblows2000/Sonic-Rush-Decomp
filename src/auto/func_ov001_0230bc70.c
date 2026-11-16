/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_0230c104();
extern u8 data_022c4560[];
s32 func_02055194();

void func_ov001_0230bc70(void) {
    func_ov001_0230c104();
    s32 t0 = (*(u32 *)((u8 *)data_022c4560 + 0x18) & 0x40);
    if (t0 == 0) {
        return;
    }
    func_02055194(t0);
}
