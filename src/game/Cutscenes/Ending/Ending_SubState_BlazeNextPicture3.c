#include <nitro/types.h>

typedef struct { s32 m[4]; } UnkStruct_022c6ef4;
extern u16 gDisplayMain[];
extern u8 data_02089264[];
void Ending_CreateBg0PictureTask(void *p);
void Ending_DrawNextPicture(void *p);
void Ending_SetSubState(void *p, void (*f)(void));
void Ending_SubState_BlazeShowPicture3(void);
void Background_Draw(void *p);
void MTX_Scale22_(UnkStruct_022c6ef4 *m, s32 a, s32 b);
void G2x_SetBGyAffine_(void *a, UnkStruct_022c6ef4 *m, s32 b, s32 c, s32 d, s32 e);

void Ending_SubState_BlazeNextPicture3(u8 *p) {
    UnkStruct_022c6ef4 m;
    gDisplayMain[0x24] = 0x10;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_BlazeShowPicture3);
    Background_Draw(p + 0x45c);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0xb00;
    MTX_Scale22_(&m, 0, 0);
    G2x_SetBGyAffine_(data_02089264, &m, 0, 0, 0, 0);
}
