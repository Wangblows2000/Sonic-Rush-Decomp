#include <nitro/types.h>

void func_ov001_022cf894(void *p);
void func_ov001_022cf820(void *p);
void func_ov001_022cf724(void);

#pragma thumb on
void SegaLogo_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    func_ov001_022cf894(q);
    func_ov001_022cf820(q);
    func_ov001_022cf724();
}
#pragma thumb reset
