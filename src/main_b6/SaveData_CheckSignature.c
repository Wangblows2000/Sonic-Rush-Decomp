#include <nitro/types.h>

extern u16 gSaveState;
extern u8 gSaveSignature[];

void func_0200f24c(u16 id);
void func_0200f23c(u16 id);
s32 func_0200fa70(s32 a, void *buf, u32 size, s32 b, s32 c, s32 d);
s32 func_0202a1a0(const void *a, const void *b, u32 n);

BOOL SaveData_CheckSignature(void) {
    u16 id = gSaveState;
    u8 buf[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    func_0200f24c(id);
    if (func_0200fa70(0, buf, 12, 0, 0, 0) == 0) {
        func_0200f23c(id);
        return FALSE;
    }
    func_0200f23c(id);
    return func_0202a1a0(buf, gSaveSignature, 12) == 0;
}
