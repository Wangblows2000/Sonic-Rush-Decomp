#include <nitro/types.h>

typedef struct { s32 m[4]; } UnkStruct_022c6ef4;
extern u16 gDisplayMain[];
extern u8 data_02089264[];
void Ending_CreateBg0PictureTask(void *p);
void Ending_DrawNextPicture(void *p);
void Ending_SetSubState(void *p, void (*f)(void));
void Ending_SubState_SonicShowPicture2(void);
void MTX_Identity22_(UnkStruct_022c6ef4 *m);
void G2x_SetBGyAffine_(void *a, UnkStruct_022c6ef4 *m, s32 b, s32 c, s32 d, s32 e);

void Ending_SubState_SonicNextPicture2(u8 *p) {
    UnkStruct_022c6ef4 m;
    gDisplayMain[0x24] = 0x10;
    Ending_CreateBg0PictureTask(p);
    Ending_DrawNextPicture(p);
    Ending_SetSubState(p, Ending_SubState_SonicShowPicture2);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0xf00;
    MTX_Identity22_(&m);
    G2x_SetBGyAffine_(data_02089264, &m, 0, 0, -0xb0, -0x20);
}
