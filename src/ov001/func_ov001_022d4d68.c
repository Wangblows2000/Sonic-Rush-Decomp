#include <nitro/types.h>

void func_ov001_022d4500(void *p);
void Touch_Disable(void);
void func_ov001_022d4d30(void *p);
void func_ov001_022d4d2c(void *p);
void func_ov001_022d4cb8(void *p);
void func_ov001_022d4220(void);

#pragma thumb on
void func_ov001_022d4d68(u8 *p) {
    void *q = *(void **)(p + 0x10);
    func_ov001_022d4500(q);
    Touch_Disable();
    func_ov001_022d4d30(q);
    func_ov001_022d4d2c(q);
    func_ov001_022d4cb8(q);
    func_ov001_022d4220();
}
#pragma thumb reset
