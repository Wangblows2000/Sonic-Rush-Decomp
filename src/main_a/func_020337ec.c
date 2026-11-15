#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; u32 unk10; u32 unk14; } UnkStruct_020337ec;
UnkStruct_020337ec *func_020336f8(void);
UnkStruct_020337ec *func_02006abc(u32 a);

void func_020337ec(u32 a, u32 b, u32 c, u32 d) {
    UnkStruct_020337ec *s = func_020336f8();
    if (func_02006abc(0) == s) return;
    s->unk8 = c;
    s->unkC = d;
    s->unk4 = 0;
    s->unk10 = a;
    s->unk14 = b;
}
