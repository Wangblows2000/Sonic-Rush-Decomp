#include <nitro/types.h>

typedef struct UnkStruct_022cfe5c {
    u8 pad[0x78];
    s32 unk78;
    s32 unk7C;
    u8 pad1[4];
    s32 unk84;
    s32 unk88;
    u8 pad2[2];
    u16 unk8E;
    u16 unk90;
} UnkStruct_022cfe5c;

typedef struct UnkStruct_022cfe5c_2 {
    u8 pad[0x18];
    u16 unk18;
    u16 unk1A;
} UnkStruct_022cfe5c_2;

extern void *data_022c4488;
extern u8 data_ov000_02365288[];
extern UnkStruct_022cfe5c data_ov000_0236a48c;
extern u32 data_ov000_02362e34[];
extern s32 data_ov000_02362f0c[][3];
extern s16 data_ov000_02362ed0[][3];
extern u16 data_ov000_02362ee4[][3];

void func_02021310(void *ctx, void *a, void *b);
UnkStruct_022cfe5c_2 *func_02021264(void *ctx, u32 id);
void func_0203473c(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203438c(void *buf);
void func_020212e0(void *ctx);

void func_ov000_022cfe5c(s32 a, s32 b) {
    UnkStruct_022cfe5c *p;
    u8 buf[0x40];
    u8 ctx[0x5c];
    UnkStruct_022cfe5c_2 *r;
    p = (UnkStruct_022cfe5c *)(u32)&data_ov000_0236a48c;
    func_02021310(ctx, data_ov000_02365288, data_022c4488);
    r = func_02021264(ctx, data_ov000_02362e34[b]);
    func_0203473c(buf, r, 0x38, 0, 2, r->unk18, r->unk1A);
    func_0203438c(buf);
    func_020212e0(ctx);
    p->unk78 = p->unk7C = data_ov000_02362f0c[a][b];
    p->unk84 = p->unk88 = data_ov000_02362ed0[a][b];
    p->unk8E = p->unk90 = data_ov000_02362ee4[a][b];
}
