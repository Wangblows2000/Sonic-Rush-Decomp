#include <nitro/types.h>

extern u16 data_022503e0[];
extern u8 *data_ov001_0231f5c0;
BOOL func_ov001_02309894(void);
void func_02055918(s32 a);
void func_ov001_023143c4(void);
void func_ov001_023147d8(void);
void func_ov001_0231323c(void);
void func_ov001_023159b4(void);
void func_ov001_0230d964(void);
void func_ov001_0230f448(void *p);
void func_ov001_023070d0(void);
void func_ov001_0230ae4c(void);
void func_ov001_02309e3c(void);

void func_ov001_0230bbd8(void) {
    u8 *g = data_ov001_0231f5c0;
    if ((data_022503e0[2] & 8) && !func_ov001_02309894()) {
        func_02055918(1);
        return;
    }
    if (*(s32 *)(g + 0x20) != 0) {
        return;
    }
    func_ov001_023143c4();
    func_ov001_023147d8();
    if (*(s32 *)(g + 0x18) != 0) {
        func_ov001_0231323c();
    }
    func_ov001_023159b4();
    func_ov001_0230d964();
    func_ov001_0230f448(*(void **)(g + 0x1c));
    func_ov001_023070d0();
    func_ov001_0230ae4c();
    func_ov001_02309e3c();
    *(s32 *)(g + 0x28) += 1;
}
