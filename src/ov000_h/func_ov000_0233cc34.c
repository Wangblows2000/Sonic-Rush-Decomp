#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    u32 flags;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u16 unk46;
} UnkStruct_0233cc34_Spr;

typedef struct {
    u8 pad0[0x900];
    u16 unk900;
    u8 pad902[0x94C - 0x902];
    void *res;
} UnkStruct_0233cc34;

extern void *data_022c4488;
extern u8 data_ov000_023689a4[];
extern u8 data_ov000_023689a8[];

void func_02021310(void *f, void *path, void *arc);
void *func_0202128c(void *path);
void func_020212e0(void *f);
void *func_02039c18(u32 a, u32 b);
void func_0203363c(UnkStruct_0233cc34_Spr *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
u16 func_0206d2d0(void *res, u32 a, u32 b);
void func_0203344c(void *p, s32 a, s32 b);

void func_ov000_0233cc34(UnkStruct_0233cc34 *w) {
    u8 f[0x60];
    u8 i;
    UnkStruct_0233cc34_Spr *s;
    u32 pal;

    func_02021310(f, data_ov000_023689a4, data_022c4488);
    w->res = func_0202128c(data_ov000_023689a8);
    func_020212e0(f);
    for (i = 0; i < 2; i++) {
        s = (UnkStruct_0233cc34_Spr *)((u8 *)w + 0x8BC) + i;
        if (i == 0) {
            pal = 0x5000200;
        } else {
            pal = 0x5000600;
        }
        func_0203363c(s, w->res, 3, 0x4004, i, 0, func_02039c18(i, 4), 0, pal);
        s->unk44 = (i == 0) ? func_0206d2d0(w->res, 3, 7) : w->unk900;
        s->unk46 = 0x15;
        func_0203344c(s, 0, 0);
        s->flags |= 0x10;
    }
}
