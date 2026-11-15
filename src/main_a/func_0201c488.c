#include <nitro/types.h>

void func_02008d1c(u32 a, u32 b, u32 c, u32 d);
void func_0200de7c(void *p);
void func_0200de64(void *p);

typedef struct { u8 unk0[8]; u8 unk8[0xC]; u8 unk14[0x10]; u32 unk24_0 : 1; s32 unk24_1 : 1; u32 pad : 30; u8 unk28[0x18]; u32 unk40; u32 unk44; } UnkStruct_0201c488;

void func_0201c488(UnkStruct_0201c488 *s) {
    func_02008d1c(s->unk44, 0, 1 << s->unk40, 0);
    if (s->unk24_1) return;
    func_0200de7c(s->unk8);
    func_0200de64(s->unk14);
    s->unk24_1 = 1;
}
