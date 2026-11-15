#include <nitro/types.h>

extern u8 data_ov001_023178f4[];
u8 *func_ov001_0230c0a4(void);
s32 func_02025edc(void *p, void *k);
void func_02025a94(void *p, s32 a);

void func_ov001_02311e64(u8 *p) {
    u8 *g = func_ov001_0230c0a4();
    s32 expect;
    if (*(u32 *)(p + 8) & 0x10) {
        expect = p[0xae];
    } else {
        expect = -1;
    }
    if (expect != func_02025edc(*(u8 **)(*(u8 **)(p + 4) + 4) + 0x40, data_ov001_023178f4)) {
        return;
    }
    func_02025a94(g + 0x1f8, 0);
    *(s32 *)(p + 0x24) = 0;
    p[0x92] = 0;
}
