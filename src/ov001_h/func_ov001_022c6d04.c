#include <nitro/types.h>

extern void *data_022b4574;
extern u8 data_0208922c[];
extern u8 data_02087d04[];
void func_ov001_022c8458(void);
void func_ov001_022c7a34(void *arg, s32 a1);

void func_ov001_022c6d04(void *arg) {
    *(s16 *)(data_0208922c + 0x48) = 16;
    *(s16 *)(data_02087d04 + 0x48) = 16;
    *(void (**)(void))((u8 *)data_022b4574 + 8) = func_ov001_022c8458;
    func_ov001_022c7a34(arg, 0);
}
