#include <nitro/types.h>

extern u16 gDisplayMain[];
void Ending_SetSubState(void *p, void (*f)(void));
void Ending_SubState_SonicShowPicture0(void);

void Ending_SubState_SonicFadeIn(void *p) {
    gDisplayMain[0x24] = 0x10;
    Ending_SetSubState(p, Ending_SubState_SonicShowPicture0);
}
