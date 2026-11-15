#include <nitro/types.h>

void func_ov001_022c6090(void);

void func_ov001_022c6160(u8 *p, void (*f)(void)) {
    if (f == NULL) {
        *(void (**)(void))(p + 4) = func_ov001_022c6090;
    } else {
        *(void (**)(void))(p + 4) = f;
    }
    *(s32 *)(p + 8) = 0;
}
