#include <nitro/types.h>

void Ending_FadeIn(void *p, s32 a, s32 b, s32 c, s32 d);
BOOL Ending_FadeOut(void *p, s32 a, s32 b, s32 c, s32 d);
void Ending_SetSubState(void *p, void (*f)(void));
void func_ov001_022c6e04(void);
void func_ov001_022c6c00(void *p);

void func_ov001_022c6e54(u8 *p) {
    if (*(s32 *)(p + 0xc) >= 5) {
        Ending_FadeIn(p, *(s32 *)(p + 0xc) - 5, 0x1e, 1, 0);
    }
    if (*(s32 *)(p + 0xc) >= 0x113 && Ending_FadeOut(p, *(s32 *)(p + 0xc) - 0x113, 0x3c, 1, 0)) {
        Ending_SetSubState(p, func_ov001_022c6e04);
        return;
    }
    func_ov001_022c6c00(p);
}
