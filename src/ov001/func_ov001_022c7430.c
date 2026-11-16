#include <nitro/types.h>

extern u8 data_02250464[];
void Ending_FadeIn(void *p, s32 a, s32 b, s32 c, s32 d);
BOOL Ending_FadeOut(void *p, s32 a, s32 b, s32 c, s32 d);
void Ending_SetSubState(void *p, void (*f)(void));
void NNS_SndPlayerStopSeq(void *p, s32 a);
void func_ov001_022c73e8(void);

void func_ov001_022c7430(u8 *p) {
    if (*(s32 *)(p + 0xc) >= 5) {
        Ending_FadeIn(p, *(s32 *)(p + 0xc) - 5, 0x3c, 1, 0);
    }
    if (*(s32 *)(p + 0xc) >= 0x3c5) {
        Ending_FadeOut(p, *(s32 *)(p + 0xc) - 0x3c5, 0xb4, 1, 1);
    }
    if (*(s32 *)(p + 0xc) == 0x3c5) {
        NNS_SndPlayerStopSeq(data_02250464, 0xb4);
    }
    if (*(s32 *)(p + 0xc) < 0x483) {
        return;
    }
    Ending_SetSubState(p, func_ov001_022c73e8);
}
