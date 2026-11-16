#include <nitro/types.h>

extern s32 data_022c4560;
extern s32 data_02087ccc;
extern u32 data_02087cb8;

void func_0204ec40(void);
void func_ov001_022c5d1c(void);
void func_020521b4(void);
void func_020522f4(void);
void func_02052168(s32 a);
void func_0205453c(s32 a, s32 b);
void func_02064644(void);
void func_02053c58(void);
void func_0206488c(void);
void func_02063f50(void);
void func_02053b04(s32 a);
void func_ov001_022e190c(s32 a, s32 b, s32 c);
void func_02037d60(void);
void *func_020372d0(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov001_022c5fa0(void);

#pragma thumb on
void func_ov001_022c6004(void) {
    data_022c4560 = 0;
    func_0204ec40();
    func_ov001_022c5d1c();
    func_020521b4();
    func_020522f4();
    func_02052168(4);
    func_0205453c(0, 0);
    func_02064644();
    func_02053c58();
    func_0206488c();
    func_02063f50();
    func_02053b04(0);
    func_ov001_022e190c(0, 0, 3);
    func_02037d60();
    data_02087ccc = 0;
    data_02087cb8 &= ~0x10;
    func_020372d0(func_ov001_022c5fa0, 0, 0, 0x5000, 0x2000, 0);
}
#pragma thumb reset
