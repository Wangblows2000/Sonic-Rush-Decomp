#include <nitro/types.h>

void func_020385e0(void);
void func_02038584(void);
void *func_020372d0(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_02038a00(BOOL on) { func_020372d0(on == FALSE ? func_020385e0 : func_02038584, 0, 0x30, 0xefff, 0xfffe, 0); }
