#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; u32 unk10; } UnkStruct_020356bc;
UnkStruct_020356bc *func_0203530c(void);
UnkStruct_020356bc *func_02006abc(u32 a);

void func_020356bc(u32 a, u32 b, u32 c) {
    UnkStruct_020356bc *s = func_0203530c();
    if (func_02006abc(0) != s) {
        s->unk8 = b;
        s->unkC = c;
        s->unk4 = 1;
        s->unk10 = a;
    }
}
