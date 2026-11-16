#include <nitro/types.h>

typedef struct {
    u16 *dst;
    u16 *src;
    void *res;
} UnkStruct_0234d0bc_Pal;

typedef struct {
    u8 pad0[5];
    u8 unk5;
    u8 pad6[0x10 - 6];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[0x44 - 0x1C];
    s16 unk44;
    s16 unk46;
    u8 pad48[0x54 - 0x48];
    u32 unk54;
    u8 pad58[0x327 - 0x58];
    u8 unk327;
    u8 pad328[0x32C - 0x328];
    u32 unk32C;
    u32 unk330;
    u8 pad334[0x376 - 0x334];
    s16 unk376;
    u8 pad378[0x3AC - 0x378];
    s16 unk3AC;
    s16 unk3AE;
    u8 pad3B0[0x3B2 - 0x3B0];
    s16 unk3B2;
    s16 unk3B4;
    s16 unk3B6;
    s16 unk3B8;
    s16 unk3BA;
    u8 pad3BC[0x3BE - 0x3BC];
    s16 unk3BE;
    s16 unk3C0;
    s16 unk3C2;
    u8 pad3C4[0x3C8 - 0x3C4];
    s16 unk3C8;
    u8 pad3CA[0x400 - 0x3CA];
    u32 unk400;
    u32 unk404;
    s32 unk408;
    s32 unk40C;
    u8 unk410;
    u8 unk411;
    u8 pad412[0x414 - 0x412];
    UnkStruct_0234d0bc_Pal pal;
    u8 pad420[0x42C - 0x420];
    u16 unk42C;
    u16 unk42E;
    u16 unk430;
    u16 unk432;
} UnkStruct_0234d0bc;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x18 - 0x14];
    u32 unk18;
} UnkStruct_0234d0bc_4560;

typedef struct {
    u8 pad0[0x14];
    u8 unk14;
    u8 pad15[0x1C - 0x15];
    s32 unk1C;
    s32 unk20;
} UnkStruct_0234d0bc_4590;

typedef struct {
    u32 v;
    u8 pad4[0x60];
} UnkStruct_0234d0bc_L;

extern UnkStruct_0234d0bc_4560 data_022c4560;
extern UnkStruct_0234d0bc_4590 data_022c4590;
extern UnkStruct_0234d0bc_L data_ov000_0236aad8[];
extern u32 gObjManagerFlags;
extern u16 data_022c4ef8;

void func_ov000_0233c00c(void);
void func_0204d63c(void);
void func_0206cdf4(UnkStruct_0234d0bc_Pal *p, s32 a, s32 b, s32 c);
void func_ov000_0233b178(u8 a);
void func_ov000_0233b158(u8 a);
void func_ov000_0234cb70(UnkStruct_0234d0bc *w);

void func_ov000_0234d0bc(UnkStruct_0234d0bc *w) {
    UnkStruct_0234d0bc_4590 *g = &data_022c4590;

    if (data_022c4560.unk10 != 1 && !(w->unk330 & 0x400000)) {
        if (g->unk14 != 0 && data_022c4560.unk10 != 2) {
            g->unk14--;
        } else {
            w->unk32C |= 0x800000;
            return;
        }
        func_0204d63c();
    } else {
        func_ov000_0233c00c();
        w->unk330 |= 0x200000;
    }
    data_ov000_0236aad8[w->unk327].v |= 1;
    data_ov000_0236aad8[w->unk327].v |= 2;
    w->x = g->unk1C << 8;
    w->y = g->unk20 << 8;
    if (!(w->unk330 & 0x400000)) {
        w->unk3C2 = 0;
        w->unk5 = 0;
        w->unk3C0 = 0;
        w->unk430 = 0x40;
        w->unk432 = 0x80;
        w->unk42E = 0x20;
        w->unk42C = 0x10;
        w->unk32C &= ~0x30000000;
        w->unk54 |= 1;
        w->unk3B6 = 0;
        w->unk3BE = 0;
        w->unk3B4 = 0;
        w->unk3B2 = 0;
        w->unk3C8 = 0;
        func_0206cdf4(&w->pal, 0, 0, 0);
        w->unk3AE = 0;
        if (data_022c4560.unk18 & 0x20) {
            w->unk3AE = 1;
        }
    } else {
        w->x = w->unk408;
        w->y = w->unk40C;
        w->z = 0;
        w->unk408 = 0;
        w->unk40C = 0;
        w->unk3B2 = w->unk376;
        w->unk3C8 = w->unk3B2;
    }
    gObjManagerFlags &= ~0x100;
    data_022c4ef8 = 0;
    w->unk32C &= ~0x1800;
    w->unk32C &= ~0x180;
    w->unk330 &= ~0x2000;
    w->unk330 &= ~0x200;
    w->unk330 &= ~0x400;
    w->unk330 &= ~0x100;
    w->unk330 &= ~0x1000;
    w->unk330 &= ~0x80000;
    w->unk330 &= ~0x100000;
    w->unk330 &= ~0x8000;
    w->unk330 &= ~0x4000;
    w->unk330 &= ~0xa0;
    w->unk330 &= ~0x50;
    func_ov000_0233b178(w->unk327);
    func_ov000_0233b158(w->unk327);
    w->unk400 = 0;
    w->unk404 = 0;
    w->unk3B8 = 0;
    w->unk3BA = 0;
    w->unk410 = 0;
    w->unk411 = 0;
    w->unk330 &= ~4;
    w->unk3AC = 0;
    w->unk44 = 0;
    w->unk46 = 0;
    func_ov000_0234cb70(w);
}
