#include <nitro/types.h>

typedef struct UnkStruct_022fd000_o {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 unk10D;
    u8 unk10E;
    u8 unk10F;
} UnkStruct_022fd000_o;

typedef struct UnkStruct_022fd000_w {
    u8 pad0[4];
    s32 type;
} UnkStruct_022fd000_w;

typedef struct UnkStruct_022fd000_p {
    u8 pad0[0x1dc];
    UnkStruct_022fd000_w *work;
} UnkStruct_022fd000_p;

typedef struct UnkStruct_022fd000 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x74 - 0x18];
    void (*state)(struct UnkStruct_022fd000 *p);
    u8 pad78[4];
    UnkStruct_022fd000_p *parent;
    u8 pad80[4];
    UnkStruct_022fd000_o *unk84;
    u8 pad88[0x1cc - 0x88];
    s32 unk1CC;
} UnkStruct_022fd000;

typedef struct UnkStruct_022fd000_c {
    u8 pad0[0x3c];
    s32 unk3C;
} UnkStruct_022fd000_c;

UnkStruct_022fd000_c *ExtraBoss_GetWork(void);
void func_ov001_022fcb08(UnkStruct_022fd000 *p, s32 n);
void func_ov001_022fcae8(UnkStruct_022fd000 *p);
void func_ov001_022fce7c(UnkStruct_022fd000 *p);
void func_ov001_022fcbd4(UnkStruct_022fd000 *p);

void func_ov001_022fd000(UnkStruct_022fd000 *p) {
    switch (p->parent->work->type) {
    case 0:
    case 1:
    case 2:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        break;
    case 3:
    case 4:
        p->state = func_ov001_022fce7c;
        p->unk1CC = 0;
        func_ov001_022fcb08(p, ExtraBoss_GetWork()->unk3C == 0 ? 0x18 : 0x1a);
        if (ExtraBoss_GetWork()->unk3C == 0) {
            p->unk84->unk10C &= ~4;
            p->unk84->unk10D &= ~4;
            p->unk84->unk10F &= ~4;
        } else {
            p->unk84->unk10C |= 4;
            p->unk84->unk10D |= 4;
            p->unk84->unk10F |= 4;
        }
        func_ov001_022fcae8(p);
        return;
    case 5:
    case 6:
    case 12:
        p->state = func_ov001_022fcbd4;
        p->flags |= 0x30000;
        return;
    }
    func_ov001_022fcae8(p);
}
