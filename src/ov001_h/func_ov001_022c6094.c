#include <nitro/types.h>

extern void *data_022b4574;
extern u8 data_0208922c[];
extern u8 data_02087d04[];
void func_ov001_022c66c4(void);
void func_ov001_022c6090(void);
void func_ov001_022c6160(void *arg, void (*fn)(void));

void func_ov001_022c6094(void *arg) {
    *(s16 *)(data_0208922c + 0x48) = -16;
    *(s16 *)(data_02087d04 + 0x48) = -16;
    *(void (**)(void))((u8 *)data_022b4574 + 8) = func_ov001_022c66c4;
    func_ov001_022c6160(arg, func_ov001_022c6090);
}
