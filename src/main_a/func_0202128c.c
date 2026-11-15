#include <nitro/types.h>

typedef struct { u8 unk0[0x58]; u32 unk58; } UnkArc_0202128c;
typedef struct { u32 unk0; u32 unk4; UnkArc_0202128c *unk8; u8 unkC[0x14]; u32 unk20; u8 unk24[0x20]; } UnkFile_0202128c;
void func_0200be70(UnkFile_0202128c *f);
BOOL func_0200bb20(UnkFile_0202128c *f, void *path);
void func_0200bad8(UnkFile_0202128c *f);

u32 func_0202128c(void *path) {
    UnkFile_0202128c f;
    u32 r = 0;
    func_0200be70(&f);
    if (func_0200bb20(&f, path)) {
        r = f.unk8->unk58 + f.unk20;
        func_0200bad8(&f);
    }
    return r;
}
