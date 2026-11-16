#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    u32 unk2C;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u16 unk46;
} UnkStruct_0231aa74_Spr;

typedef struct {
    u8 pad0[0x20C];
    UnkStruct_0231aa74_Spr spr[2];
    void *unk29C;
    u32 unk2A0;
    s32 unk2A4;
} UnkStruct_0231aa74;

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkStruct_0231aa74_Tbl;

typedef struct {
    u8 pad[0x5C];
} UnkStruct_0231aa74_Ctx;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367198[];
extern u8 data_ov000_0236719c[];
extern u8 data_ov000_023671b8[];
extern UnkStruct_0231aa74_Tbl data_ov000_02364494[];

void func_02021310(UnkStruct_0231aa74_Ctx *ctx, void *a, void *b);
void *func_0202128c(void *name);
void *func_02033fbc(void *name, s32 a);
void func_020212e0(UnkStruct_0231aa74_Ctx *ctx);
void *func_02039c18(u8 i, s32 a);
void func_0203363c(UnkStruct_0231aa74_Spr *spr, void *res, u16 idx, u32 fl, u8 i, s32 a, void *vram, s32 b, void *pal);
u16 func_0206d2d0(void *res, u16 idx, s32 a);
void func_0203344c(UnkStruct_0231aa74_Spr *spr, s32 a, s32 b);

void func_ov000_0231aa74(UnkStruct_0231aa74 *w) {
    UnkStruct_0231aa74_Ctx ctx;
    u8 i;
    u16 idx;
    u32 fl;

    func_02021310(&ctx, data_ov000_02367198, data_ov000_02369ba0);
    w->unk29C = func_0202128c(data_ov000_0236719c);
    if (w->unk29C == NULL) {
        w->unk29C = func_02033fbc(data_ov000_023671b8, 0);
        w->unk2A0 |= 1;
    }
    func_020212e0(&ctx);
    idx = data_ov000_02364494[w->unk2A4].unk0;
    fl = data_ov000_02364494[w->unk2A4].unk8 | 0x4004;
    for (i = 0; i < 2; i++) {
        UnkStruct_0231aa74_Spr *spr = &w->spr[i];
        void *pal;
        if (i == 0) {
            pal = (void *)0x5000200;
        } else {
            pal = (void *)0x5000600;
        }
        func_0203363c(spr, w->unk29C, idx, fl, i, 0, func_02039c18(i, 0x14), 0, pal);
        spr->unk44 = (i == 0) ? func_0206d2d0(w->unk29C, idx, 0xc) : w->spr[0].unk44;
        spr->unk46 = 0x13;
        func_0203344c(spr, 0, 0);
        spr->unk2C |= 0x10;
    }
}
