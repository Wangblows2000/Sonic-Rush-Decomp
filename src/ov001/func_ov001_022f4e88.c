#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231f520;
extern s32 data_ov001_0231f524;
void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);
void func_ov001_022fad38(void *p, s32 a, s32 b, s32 c, u16 d);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov001_022f4d74(void);

void func_ov001_022f4e88(u8 *p) {
    u8 *r = *(u8 **)(p + 0x84);
    u8 *q;
    if (!(r[0x10c] & 2)) {
        return;
    }
    q = *(u8 **)(p + 0x1dc);
    *(void (**)(void))(p + 0x74) = func_ov001_022f4d74;
    func_02031a68(r, 0, data_ov001_0231f068, 0x15, 0);
    func_ov001_022fad38(p, *(s32 *)(q + 0x38) - data_ov001_0231f524, -(*(s32 *)(q + 0x3c) + data_ov001_0231f520), *(s32 *)(q + 0x40),
                        (*(s32 *)(p + 0x1cc) - 1) * *(u16 *)(p + 0x1d4) + 0x9555);
    func_020366f4(0, 0, 0xd2);
}
