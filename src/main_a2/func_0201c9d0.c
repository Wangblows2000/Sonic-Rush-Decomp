#include <nitro/types.h>

void func_02008cac(u32 a, u32 b, u32 c, u32 d);
u32 func_020091ac(void);
void func_0200927c(u32 a);
void func_020091ec(u32 a);
BOOL func_020063f4(void *queue, void *msg, s32 flags);
void func_0201b50c(u32 a);
void func_0201b54c(void *p);
void func_0201b4a8(s32 a);
void func_02008a28(u32 a, u32 b, u32 c, u32 d);

typedef struct { u32 unk0; u32 unk4; u8 unk8[0x18]; void *unk20; u32 unk24; u32 unk28; s32 unk2C; } UnkStruct_0201c9d0;
extern UnkStruct_0201c9d0 data_02083e10;
extern u8 data_02083dfc[];

void func_0201c9d0(void) {
    UnkStruct_0201c9d0 *p = &data_02083e10;
    BOOL has;
    if (p->unk0 == 0) return;
    has = p->unk2C >= 0;
    func_02008cac(p->unk24, p->unk28, has ? 1 << p->unk2C : 0, 0);
    if (has) {
        u32 x = func_020091ac();
        func_0200927c(1);
        void *q;
        func_020091ec(x);
        q = data_02083dfc;
        while (func_020063f4(q, NULL, 0)) {
        }
    }
    if (p->unk28) func_0201b50c(p->unk28);
    if (p->unk20) func_0201b54c(p->unk20);
    if (has) func_0201b4a8(p->unk2C);
    if (p->unk4 == 1) func_02008a28(0, 0, 0, 0);
    p->unk0 = 0;
}
