#include <nitro/types.h>

extern u32 data_ov000_0236a670[];
extern u32 data_ov000_0236a65c;

void func_ov000_0232644c(u32 *e) {
    u32 *p = data_ov000_0236a670;
    u8 i;

    for (i = 0; i < 32; i++) {
        if (p == e) {
            data_ov000_0236a65c &= ~(1 << i);
            return;
        }
        p++;
    }
}
