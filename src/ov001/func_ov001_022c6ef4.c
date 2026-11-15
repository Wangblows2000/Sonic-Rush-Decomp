#include <nitro/types.h>

typedef struct { s32 m[4]; } UnkStruct_022c6ef4;
extern u16 data_0208922c[];
extern u8 data_02089264[];
void func_ov001_022c6944(void *p);
void func_ov001_022c6878(void *p);
void func_ov001_022c7a34(void *p, void (*f)(void));
void func_ov001_022c6e54(void);
void func_0203438c(void *p);
void func_02000ccc(UnkStruct_022c6ef4 *m, s32 a, s32 b);
void func_02003d94(void *a, UnkStruct_022c6ef4 *m, s32 b, s32 c, s32 d, s32 e);

void func_ov001_022c6ef4(u8 *p) {
    UnkStruct_022c6ef4 m;
    data_0208922c[0x24] = 0x10;
    func_ov001_022c6944(p);
    func_ov001_022c6878(p);
    func_ov001_022c7a34(p, func_ov001_022c6e54);
    func_0203438c(p + 0x45c);
    *(vu32 *)0x04000000 = (*(vu32 *)0x04000000 & ~0x1f00) | 0xb00;
    func_02000ccc(&m, 0, 0);
    func_02003d94(data_02089264, &m, 0, 0, 0, 0);
}
