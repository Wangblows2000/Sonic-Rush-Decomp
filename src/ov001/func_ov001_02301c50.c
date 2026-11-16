#include <nitro/types.h>

void func_ov001_02302398(void *p);
void func_ov001_023024d0(void *p, s32 a);
void func_ov001_022fcaa4(void *p, s32 x, s32 y, s32 z);
void func_ov001_022fc9e4(void *p, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov001_02301ff4(void *p);

void func_ov001_02301c50(u8 *p) {
    func_ov001_02302398(p);
    *(s32 *)(p + 0x1c8) -= 1;
    if (*(s32 *)(p + 0x1c8) <= 0) {
        func_ov001_023024d0(p, 0xa);
        func_ov001_022fcaa4(p, *(s32 *)(p + 0x18), *(s32 *)(p + 0x1c), 0);
        func_ov001_022fc9e4(p, *(s32 *)(p + 0x18), *(s32 *)(p + 0x1c), 0);
        Sound_PlaySeqArc(0, 0, 0x12);
    }
    func_ov001_02301ff4(p);
}
