#include <nitro/types.h>

extern u16 gDisplayMain[];
void Ending_FadeIn(void *p, s32 a, s32 b, s32 c, s32 d);
BOOL Ending_FadeOut(void *p, s32 a, s32 b, s32 c, s32 d);
void Ending_SetSubState(void *p, void (*f)(void));
void Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void Ending_SubState_BlazeNextPicture3(void);
void EndingBgLayersShow_Main(void);

void Ending_SubState_BlazeShowPicture2(u8 *p) {
    if (*(s32 *)(p + 0xc) >= 5) {
        Ending_FadeIn(p, *(s32 *)(p + 0xc) - 5, 0x3c, 1, 0);
    }
    if (*(s32 *)(p + 0xc) >= 0x113 && Ending_FadeOut(p, *(s32 *)(p + 0xc) - 0x113, 0x1e, 1, 0)) {
        Ending_SetSubState(p, Ending_SubState_BlazeNextPicture3);
        return;
    }
    if (*(s32 *)(p + 0x698) == *(s32 *)(p + 0x69c)) {
        return;
    }
    gDisplayMain[4] = 0;
    gDisplayMain[5] = 0;
    Task_Create(EndingBgLayersShow_Main, 0, 0, 0xf400, 0x1173, 0);
}
