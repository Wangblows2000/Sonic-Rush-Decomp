#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; u32 unk10; } UnkStruct_020337a4;
UnkStruct_020337a4 *func_020336f8(void);
UnkStruct_020337a4 *func_02006abc(u32 a);

void func_020337a4(u32 a, u32 b, u32 c) {
    UnkStruct_020337a4 *s = func_020336f8();
    if (func_02006abc(0) != s) {
        s->unk8 = b;
        s->unkC = c;
        s->unk4 = 1;
        s->unk10 = a;
    }
}
