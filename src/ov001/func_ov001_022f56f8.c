#include <nitro/types.h>

extern void *data_ov001_0231f068;
extern s32 data_ov001_0231d4c0[];
void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);
void func_ov001_022fb5ec(void *p);
void func_ov001_022f55e4(void *p);
void func_02036754(s32 a, s32 b, s32 c);
void func_020366f4(s32 a, s32 b, s32 c);

void func_ov001_022f56f8(u8 *p) {
    u8 *r = *(u8 **)(p + 0x84);
    if (!(r[0x10c] & 2)) {
        return;
    }
    *(void (**)(void *))(p + 0x74) = func_ov001_022f55e4;
    func_02031a68(r, 0, data_ov001_0231f068, data_ov001_0231d4c0[*(s32 *)(p + 0x1cc)] + 2, 0);
    if (*(s32 *)(p + 0x1cc) == 1) {
        *(s32 *)(p + 0x50) = -0x6000;
    }
    func_ov001_022fb5ec(*(void **)(*(u8 **)(p + 0x1dc) + 0x10));
    func_ov001_022f55e4(p);
    func_02036754(0, 0, 0xad);
    func_020366f4(0, 0, 0xd0);
}
