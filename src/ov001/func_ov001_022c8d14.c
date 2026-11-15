#include <nitro/types.h>

typedef struct { u8 a; u8 b; s32 c; } UnkStruct_022c8d14;
void func_ov001_022c8820(UnkStruct_022c8d14 *s);

#pragma thumb on
void func_ov001_022c8d14(u8 *p) {
    UnkStruct_022c8d14 s;
    if (*(s32 *)(p + 0xc) == 0) {
        s.a = 0;
        s.b = 1;
        s.c = 0;
    } else {
        s.a = 1;
        s.b = 0;
        s.c = 1;
    }
    func_ov001_022c8820(&s);
}
#pragma thumb reset
