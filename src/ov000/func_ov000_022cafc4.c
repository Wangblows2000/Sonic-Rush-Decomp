#include <nitro/types.h>

BOOL func_ov000_022cc140(void);
void func_ov000_022cbfc8(void);

void func_ov000_022cafc4(void) {
    if (func_ov000_022cc140() == 0)
        return;
    func_ov000_022cbfc8();
}
