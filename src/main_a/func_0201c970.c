#include <nitro/types.h>

void func_02008cac(u32 a, u32 b, u32 c, u32 d);
u32 func_020091ac(void);
void func_0200927c(u32 a);
void func_020091ec(u32 a);

typedef struct { u32 unk0; u8 unk4[0x20]; u32 unk24; u32 unk28; s32 unk2C; } UnkStruct_0201c970;
extern UnkStruct_0201c970 data_02083e10;

void func_0201c970(void) {
    UnkStruct_0201c970 *p = &data_02083e10;
    u32 x;
    if (p->unk0 == 0) return;
    func_02008cac(p->unk24, p->unk28, p->unk2C >= 0 ? 1 << p->unk2C : 0, 0);
    x = func_020091ac();
    func_0200927c(1);
    func_020091ec(x);
}
