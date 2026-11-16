#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022e6dec;

typedef struct {
    s32 x;
    s32 y;
} UnkA_022e6dec;

typedef struct {
    u8 pad0[0xf4c];
    UnkSpr_022e6dec spr;
} UnkB_022e6dec;

typedef struct {
    u8 pad0[8];
    s32 x;
    s32 y;
} UnkC_022e6dec;

typedef struct {
    u8 pad0[4];
    UnkA_022e6dec a;
    u8 padc[0x674 - 0xc];
    UnkB_022e6dec b;
    u8 padb[0x1994 - 0x674 - sizeof(UnkB_022e6dec)];
    UnkC_022e6dec c;
} UnkSub_022e6dec;

typedef struct {
    s32 unk0;
    u8 pad4[4];
    s32 unk8;
    u8 padc[0x64 - 0xc];
    s32 unk64;
    s32 unk68;
    s32 unk6c;
    s32 unk70;
    u8 pad74[0x8c - 0x74];
    s32 unk8c;
    u8 pad90[0xa4 - 0x90];
    UnkSub_022e6dec *sub;
} UnkStruct_022e6dec;

void func_ov001_022e54b4(UnkA_022e6dec *a);
void func_ov001_022e560c(UnkA_022e6dec *a);
void func_ov001_022e4b5c(UnkB_022e6dec *b);
void func_ov001_022e3b24(UnkC_022e6dec *c);
void func_ov001_022e397c(UnkC_022e6dec *c);
void func_0203344c(UnkSpr_022e6dec *s, s32 a, s32 b);
void func_02032d2c(UnkSpr_022e6dec *s);

#pragma thumb on
void func_ov001_022e6dec(UnkStruct_022e6dec *p) {
    UnkSub_022e6dec *sub;
    UnkA_022e6dec *a;
    UnkB_022e6dec *b;
    UnkC_022e6dec *c;
    UnkSpr_022e6dec *s;

    sub = p->sub;
    a = &sub->a;
    b = &sub->b;
    c = &sub->c;
    c->x = p->unk64 - (p->unk6c << 12);
    c->y = p->unk68 - (p->unk70 << 12);
    sub->a.x = p->unk6c;
    a->y = p->unk70;
    func_ov001_022e54b4(a);
    func_ov001_022e560c(a);
    func_ov001_022e4b5c(b);
    func_ov001_022e3b24(c);
    if (p->unk0 == 1 && p->unk8 < 2) {
        if (p->unk8 == 1 && p->unk8c == 1) {
            func_ov001_022e397c(c);
            return;
        }
        s = &b->spr;
        s->x = c->x >> 12;
        s->y = c->y >> 12;
        func_0203344c(s, 0, 0);
        func_02032d2c(s);
        return;
    }
    func_ov001_022e397c(c);
}
#pragma thumb reset
