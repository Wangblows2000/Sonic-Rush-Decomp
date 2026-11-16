#include <nitro/types.h>

typedef struct { s32 m[4]; } UnkStruct_022c6ef4;
extern u16 gDisplayMain[];
extern u8 data_02089264[];
void func_ov001_022c6944(void *p);
void func_ov001_022c6878(void *p);
void Ending_SetSubState(void *p, void (*f)(void));
void func_ov001_022c7678(void);
void MTX_Identity22_(UnkStruct_022c6ef4 *m);
void G2x_SetBGyAffine_(void *a, UnkStruct_022c6ef4 *m, s32 b, s32 c, s32 d, s32 e);

void func_ov001_022c77e4(u8 *p) {
    UnkStruct_022c6ef4 m;
    gDisplayMain[0x24] = 0x10;
    func_ov001_022c6944(p);
    func_ov001_022c6878(p);
    Ending_SetSubState(p, func_ov001_022c7678);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0xf00;
    MTX_Identity22_(&m);
    G2x_SetBGyAffine_(data_02089264, &m, 0, 0, -0xb0, -0x20);
}
