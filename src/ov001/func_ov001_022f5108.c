#include <nitro/types.h>

extern void *data_ov001_0231f068;
void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);
void func_ov001_022f3b50(void *p);
void func_ov001_022f6450(void *p);
void func_ov001_022f5090(void);

void func_ov001_022f5108(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    func_ov001_022f3b50(p);
    if (!(q[0x10c] & 2)) {
        return;
    }
    *(s32 *)(p + 0x1cc) -= 1;
    if (*(s32 *)(p + 0x1cc) <= 0) {
        func_ov001_022f6450(p);
        return;
    }
    *(void (**)(void))(p + 0x74) = func_ov001_022f5090;
    func_02031a68(q, 0, data_ov001_0231f068, 0, 0);
    q[0x10c] |= 4;
    *(s32 *)(p + 0x1c8) = 0x28;
}
