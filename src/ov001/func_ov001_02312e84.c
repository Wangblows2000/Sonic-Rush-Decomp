#include <nitro/types.h>

void *func_ov001_023070fc(s32 a, s32 x, s32 y, s32 z);
void func_ov001_0231066c(void *a, void *b);
void func_ov001_0230f4bc(void *p);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov001_023122d8(void *p, s32 v);
void func_ov001_0231599c(s32 v);

void func_ov001_02312e84(u8 *p, u8 *o) {
    u8 *q;
    if (*(s32 *)(p + 0x74) > 0) {
        return;
    }
    switch (*(s16 *)(o + 0x68)) {
    case 0xd:
    case 0xe:
        q = func_ov001_023070fc(1, *(s32 *)(o + 0x20), *(s32 *)(o + 0x24), *(s32 *)(o + 0x28));
        if (q != NULL) {
            *(s32 *)(*(u8 **)(q + 0xac) + 0x114) = 0x2000;
            func_ov001_0231066c(NULL, q);
        }
        func_ov001_0230f4bc(o);
        func_02036754(0, 0x10, 0x11);
        break;
    }
    func_ov001_023122d8(p, 0x10);
    func_ov001_0231599c(2);
}
