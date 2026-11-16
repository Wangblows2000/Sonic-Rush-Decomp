#include <nitro/types.h>

extern u8 data_ov000_02369ac0[];
extern u32 data_0208fe70;
void func_ov000_022ddd4c();
s32 func_ov000_022db900();
s32 func_ov000_022dbc4c();
s32 func_ov000_022dbb7c();
s32 func_ov000_022dba50();
s32 func_ov000_022dbab4();

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_022dde20(u8 *p0) {
    *(u32 *)((u8 *)data_ov000_02369ac0 + 0x4) = 0x18;
    *(u32 *)((u8 *)p0 + 0x220) = (u32)func_ov000_022ddd4c;
    *(u32 *)((u8 *)p0 + 0x224) = 0;
    func_ov000_022db900(p0, 0x60, 0, 0x18);
    *(u32 *)((u8 *)p0 + 0xbc4) = 0x80;
    *(u32 *)((u8 *)p0 + 0xbc8) = -0x600;
    *(u32 *)((u8 *)p0 + 0xbcc) = -0x600;
    *(u32 *)((u8 *)p0 + 0xbec) = Rand() & 1;
    if ((*(u32 *)((u8 *)p0 + 0x21c) & 2) == 0) {
        return;
    }
    func_ov000_022dbc4c(0);
    func_ov000_022dbb7c(0);
    func_ov000_022dba50(0);
    func_ov000_022dbab4(0);
}
