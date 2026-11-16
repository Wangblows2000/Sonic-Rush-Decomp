#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[8];
    u16 unk8;
    u8 padA[0x24 - 0xA];
    u8 unk24[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x138 - 0x54];
} UnkStruct_022e1684_Sub;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    UnkStruct_022e1684_Sub unk210[9];
    u8 padD08[0xD50 - 0xD08];
    u16 unkD50;
    u16 unkD52;
    u8 padD54[0xD5C - 0xD54];
    u16 unkD5C;
    u8 padD5E[0xF4A - 0xD5E];
    u16 unkF4A;
    s16 unkF4C;
    u8 padF4E[0xF66 - 0xF4E];
    s16 unkF66;
    s16 unkF68;
    s16 unkF6A;
    s16 unkF6C;
    u8 padF6E[0x1044 - 0xF6E];
    u8 unk1044[0x1074 - 0x1044];
    u8 unk1074[0x1098 - 0x1074];
    u8 unk1098[0x10C8 - 0x1098];
    u8 unk10C8[0x10F0 - 0x10C8];
    void *unk10F0;
    void *unk10F4;
    void *unk10F8;
    void *unk10FC;
    void *unk1100;
    void *unk1104;
    void *unk1108;
    void *unk110C;
    void *unk1110;
    s32 unk1114;
} UnkStruct_022e1684;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_022e1684 *work;
} UnkStruct_022e1684_Task;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_022e1684_Task *data_022b4574;
extern u32 data_022c4ef0;
extern s16 data_0206e470[];
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_022e1684 *data_ov000_02369b4c;
extern void *data_ov000_02369b50[];
extern u8 data_ov000_023657e0[], data_ov000_023636f4[];

void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000e98(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_ov000_022e1f88(UnkStruct_022e1684 *w);
void func_020316f4(void *s);
void func_020315e0(void *s);
void func_02025268(s32 a, s32 *b, s32 c);
void func_02025a94(void *a, void *b);
void func_ov000_022e4568(UnkStruct_022e1684 *w);
void func_ov000_022e1474(UnkStruct_022e1684 *w);
void func_ov000_022e1308(UnkStruct_022e1684 *w);
void func_ov000_022e1078(UnkStruct_022e1684 *w);
void func_ov000_022c5d4c(void *a, void *b, void *c, void *d, s32 e);

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void func_ov000_022e1684(void) {
    u32 mtx[9];
    s32 t1;
    s32 t2;
    s32 t3;
    UnkStruct_022e1684 *w = data_022b4574->work;
    s32 i;
    UnkStruct_022e1684_Sub *sub;
    s32 idx;
    u32 flags;

    if (!(data_022c4ef0 & 1)) {
        w->unkD50 += 1.5170370370370372;
        w->unkD52 += 6.674962962962963;
        sub = &w->unk210[1];
        idx = (s32)(w->unkD50 & 0xffff) >> 4;
        MTX_RotY33_(sub->unk24, SIN_IDX(idx), COS_IDX(idx));
        sub = &w->unk210[2];
        idx = (s32)(w->unkD52 & 0xffff) >> 4;
        MTX_RotY33_(sub->unk24, SIN_IDX(idx), COS_IDX(idx));
        func_ov000_022e1f88(w);
        w->unkF68 += w->unkF6A;
        if (w->unkF6A >= 0) {
            if (w->unkF68 > w->unkF6C)
                w->unkF68 = w->unkF6C;
        } else {
            if (w->unkF68 < w->unkF6C)
                w->unkF68 = w->unkF6C;
        }
        w->unkF66 += w->unkF68;
    }
    if (!(w->unk20C & 1)) {
        sub = w->unk210;
        for (i = 0; i <= 2; i++) {
            func_020316f4(sub);
            func_020315e0(sub);
            sub++;
        }
    }
    func_020315e0(&w->unk210[w->unkD5C + 3]);
    if (w->base.unk5C & 0x20)
        return;
    sub = &w->unk210[6];
    sub->unk48 = w->base.unk10 << 4;
    sub->unk4C = -w->base.unk14 << 4;
    sub->unk50 = w->base.unk18 << 4;
    idx = (s32)(w->unkF4A & 0xffff) >> 4;
    func_02000e98(sub->unk24, SIN_IDX(idx), COS_IDX(idx));
    if (w->unkF4C != 0) {
        idx = (s32)((u16)w->unkF4C & 0xffff) >> 4;
        MTX_RotY33_(mtx, SIN_IDX(idx), COS_IDX(idx));
        func_02000f34(sub->unk24, mtx, sub->unk24);
    }
    func_020316f4(sub);
    func_020315e0(sub);
    switch (data_ov000_02369ac0.unk4) {
    case 0x1f:
    case 0x21:
        t1 = 0x1b;
        func_02025268(0x14, &t1, 1);
        func_02025a94(w->unk1098, w->unk10C8);
    case 0x20:
        t2 = 2;
        func_02025268(0x10, &t2, 1);
        t3 = 0x1c;
        func_02025268(0x14, &t3, 1);
        func_02025a94(w->unk1044, w->unk1074);
        break;
    }
    if (w->unkF68 < 0x238e) {
        sub = &w->unk210[7];
        idx = (s32)((u16)w->unkF66 & 0xffff) >> 4;
        MTX_RotY33_(sub->unk24, SIN_IDX(idx), COS_IDX(idx));
    } else {
        sub = &w->unk210[8];
        idx = (s32)((u16)(w->unkF66 >> 1) & 0xffff) >> 4;
        MTX_RotY33_(sub->unk24, SIN_IDX(idx), COS_IDX(idx));
    }
    sub->unk8 = 0x1e;
    func_020316f4(sub);
    func_020315e0(sub);
    sub->unk8 = 0x1d;
    func_020316f4(sub);
    func_020315e0(sub);
    flags = w->unk20C;
    if (flags & 4) {
        func_ov000_022e4568(w);
        func_ov000_022e1474(w);
        return;
    }
    if (flags & 8) {
        func_ov000_022e1308(w);
        return;
    }
    if (flags & 0x10) {
        func_ov000_022e1078(w);
        return;
    }
    if (w->unk1114 == 0)
        return;
    func_ov000_022c5d4c(data_ov000_02369b50[6], data_ov000_023657e0, data_ov000_02369b4c->unk110C, data_ov000_023636f4, 1);
    w->unk1114 = 0;
}
