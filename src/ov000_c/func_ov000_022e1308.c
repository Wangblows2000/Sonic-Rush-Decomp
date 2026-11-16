#include <ov000_c/stageobj.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[2];
    u16 unkA;
    u8 padC[0x58 - 0xC];
} UnkStruct_Node;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0xD5E - 0x210];
    u16 unkD5E;
    u8 padD60[0xE68 - 0xD60];
    UnkStruct_Node unkE68;
    UnkStruct_Node unkEC0;
    u8 padF18[0x1068 - 0xF18];
    s32 unk1068;
    s32 unk106C;
    u8 pad1070[0x110C - 0x1070];
    void *unk110C;
    u8 pad1110[0x1114 - 0x1110];
    s32 unk1114;
} UnkStruct_Boss;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_02369b50;

extern UnkStruct_Boss *data_ov000_02369b4c;
extern UnkStruct_02369b50 data_ov000_02369b50;
extern u8 data_ov000_023657e0[];
extern u8 data_ov000_023636f4[];

void func_0206e080(UnkStruct_Node *n);
void func_ov000_022c5d4c(void *a, void *b, void *c, void *d, s32 e);

void func_ov000_022e1308(UnkStruct_Boss *w) {
    UnkStruct_Node *n = &w->unkE68;
    s16 x = w->unk1068 >> 12;
    s16 y = -(w->unk106C >> 12);

    if (n->unkA & 4) {
        n->unk0 = x;
        n->unk4 = y;
        n->unkA &= ~3;
        if ((w->unkD5E == 0 && (w->unk20C & 0x100)) || (w->unkD5E != 0 && !(w->unk20C & 0x100)))
            n->unkA |= 1;
        func_0206e080(n);
    }
    n = &w->unkEC0;
    if (n->unkA & 4) {
        n->unk0 = x;
        n->unk4 = y;
        n->unkA &= ~3;
        if ((w->unkD5E == 0 && (w->unk20C & 0x100)) || (w->unkD5E != 0 && !(w->unk20C & 0x100)))
            n->unkA |= 1;
        func_0206e080(n);
    }
    if (w->unk1114 == 0)
        return;
    func_ov000_022c5d4c(data_ov000_02369b50.unk18, data_ov000_023657e0, data_ov000_02369b4c->unk110C, data_ov000_023636f4, 1);
    w->unk1114 = 0;
}
