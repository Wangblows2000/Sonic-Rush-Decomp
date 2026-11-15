#include <nitro/types.h>

extern u16 data_0208922c[];
void func_ov001_022c7a34(void *p, void (*f)(void));
void func_ov001_022c7940(void);

void func_ov001_022c79d4(void *p) {
    data_0208922c[0x24] = 0x10;
    func_ov001_022c7a34(p, func_ov001_022c7940);
}
