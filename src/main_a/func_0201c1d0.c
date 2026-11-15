#include <nitro/types.h>

void func_02008cac(u32 a, u32 b, u32 c, u32 d);
u32 func_020091ac(void);
void func_0200927c(u32 a);
void func_020091ec(u32 a);

typedef struct { u8 unk0[0x24]; u32 unk24_0 : 1; s32 unk24_1 : 1; u32 pad : 30; u8 unk28[0x18]; u32 unk40; u32 unk44; } UnkStruct_0201c1d0;

void func_0201c1d0(UnkStruct_0201c1d0 *s) {
    u32 x;
    if (!s->unk24_1) return;
    func_02008cac(s->unk44, 0, 1 << s->unk40, 0);
    x = func_020091ac();
    func_0200927c(1);
    func_020091ec(x);
}
