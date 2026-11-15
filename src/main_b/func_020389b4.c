#include <nitro/types.h>

void func_02038668(void);
void func_0203863c(void);
void *func_020372d0(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_020389b4(BOOL on) { func_020372d0(on == FALSE ? func_02038668 : func_0203863c, 0, 0x30, 0xefff, 0xfffe, 0); }
