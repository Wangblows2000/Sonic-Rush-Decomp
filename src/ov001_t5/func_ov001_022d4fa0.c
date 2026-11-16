#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022d4fa0;

extern UnkBright_022d4fa0 data_0208926c;
extern UnkBright_022d4fa0 data_02087d44;
extern s32 data_02087ccc;
extern u32 data_02087cb8;

s32 func_ov001_022d4f30(void);
void func_ov001_022d4514(void);
void func_ov001_022d44f4(s32 a);

#pragma thumb on
void func_ov001_022d4fa0(void) {
    s32 r;

    if (data_0208926c.bright != 0x10 && data_0208926c.bright != -0x10) {
        data_0208926c.bright = -0x10;
        data_02087d44.bright = -0x10;
    }
    r = func_ov001_022d4f30();
    func_ov001_022d4514();
    func_ov001_022d44f4(r);
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
}
#pragma thumb reset
