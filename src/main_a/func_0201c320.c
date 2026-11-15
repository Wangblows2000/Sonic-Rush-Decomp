#include <nitro/types.h>

void func_02008cac(u32 a, u32 b, u32 c, u32 d);
void func_0200de4c(void *p);
void func_0200de34(void *p);
u32 func_020091ac(void);
void func_0200927c(u32 a);
void func_020091ec(u32 a);
void func_0201c2ec(void *s);

typedef struct { u8 unk0[8]; u8 unk8[0xC]; u8 unk14[0x10]; u32 unk24_0 : 1; s32 unk24_1 : 1; u32 pad : 30; u8 unk28[0x18]; u32 unk40; u32 unk44; } UnkStruct_0201c320;

void func_0201c320(UnkStruct_0201c320 *s) {
    if (s->unk24_1) {
        u32 x;
        func_02008cac(s->unk44, 0, 1 << s->unk40, 0);
        func_0200de4c(s->unk8);
        func_0200de34(s->unk14);
        s->unk24_1 = 0;
        x = func_020091ac();
        func_0200927c(1);
        func_020091ec(x);
    }
    func_0201c2ec(s);
}
