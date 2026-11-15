#include <nitro/types.h>

extern void *data_ov001_0231f068;
void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);
s32 func_ov001_022fd4ac(void);
void func_ov001_022f3c30(void *p);
void func_ov001_022f60a4(void);
void func_ov001_022f5eb4(void);

void func_ov001_022f612c(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    *(s32 *)(p + 0x1cc) = 2;
    if (func_ov001_022fd4ac() != 0) {
        *(void (**)(void))(p + 0x74) = func_ov001_022f60a4;
        func_02031a68(q, 0, data_ov001_0231f068, 0, 0);
        q[0x10c] |= 4;
        *(s32 *)(q + 0x114) = 0x1000;
        func_ov001_022f3c30(p);
        return;
    }
    *(void (**)(void))(p + 0x74) = func_ov001_022f5eb4;
    func_02031a68(q, 0, data_ov001_0231f068, 5, 0);
    q[0x10c] &= ~4;
    *(s32 *)(q + 0x114) = 0x1000;
}
