#include <nitro/types.h>

extern u32 data_0208fe70;
void func_ov001_022e023c(void);
void func_ov001_022dfeec(void);
void func_ov001_022dfe20(void);
void func_ov001_022dfb48(void *p, void *q);
void func_ov001_022dfa3c(void *p, void *q);
void func_ov001_022df5f0(void *p);
void func_ov001_022df474(void *p);
static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return data_0208fe70 >> 16;
}

void func_ov001_022e034c(u8 *p) {
    *(s16 *)(p + 0xc) -= 1;
    if (*(s16 *)(p + 0xc) <= 0) {
        *(u32 *)(p + 4) |= 0x10;
        *(void (**)(void))p = func_ov001_022e023c;
    }
    if (*(u32 *)(p + 4) & 1) {
        func_ov001_022dfeec();
        func_ov001_022dfb48(p, p + 0x14);
    } else {
        func_ov001_022dfe20();
        func_ov001_022dfa3c(p, p + 0x14);
    }
    *(u32 *)(p + 4) ^= 1;
    if (*(s32 *)(p + 0x10) == 2) {
        if ((Rand() & 7) == 0) {
            func_ov001_022df5f0(p);
        }
    } else {
        if ((Rand() & 3) == 0) {
            func_ov001_022df5f0(p);
        }
    }
    func_ov001_022df474(p);
}
