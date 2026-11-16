#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
    u32 pad24;
    u32 unk28;
} UnkFile_020524d4;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0xc];
} UnkHdr_020524d4;

extern u32 data_02250434;
extern u32 data_02250438;

s32 func_0200b99c(void *f, void *buf, u32 size);
BOOL func_0200b930(void *f, u32 pos, u32 whence);
void func_020356bc(void *p, u32 a, u32 b);
void func_02035754(void);

void func_020524d4(UnkFile_020524d4 *f)
{
    UnkHdr_020524d4 h;
    u8 buf[0x208];
    u32 b;
    u32 a;

    func_0200b99c(f, &h, 0x20);
    func_0200b930(f, h.unk10, 0);
    func_0200b99c(f, buf, 0x204);
    b = data_02250438;
    a = data_02250434;
    data_02250434 = 0;
    data_02250438 = 0;
    func_020356bc(buf, 0, 0x05000400);
    func_02035754();
    data_02250438 = b;
    data_02250434 = a;
    f->unk28 = f->unk20;
}
