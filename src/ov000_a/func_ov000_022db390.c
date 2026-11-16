#include <nitro/types.h>

typedef struct UnkStruct_022db390_a {
    u8 pad[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad1[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad2[0x10f - 0x54];
    u8 unk10F;
    u8 pad3[0x138 - 0x110];
} UnkStruct_022db390_a;

typedef struct UnkStruct_022db390 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x54 - 0x1c];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*unk68)(void);
    u8 pad1b[0x70 - 0x6c];
    void (*unk70)(void);
    u8 pad2[0x214 - 0x74];
    void *unk214;
    u8 pad3[0x21e - 0x218];
    u16 unk21E;
    u8 pad4[0x22c - 0x220];
    s32 unk22C;
    void *unk230;
    void *unk234;
    s32 unk238;
    UnkStruct_022db390_a unk23C;
} UnkStruct_022db390;

typedef struct UnkTask_022db390 {
    u8 pad[0x10];
    UnkStruct_022db390 *unk10;
} UnkTask_022db390;

typedef struct UnkStruct_022db390_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022db390_7;

typedef struct { u8 data[0x5c]; } UnkArc_022db390;

extern void *data_022c4488;
extern s32 data_022c4560;
extern UnkStruct_022db390 *data_ov000_02369b14;
extern UnkStruct_022db390_7 data_ov000_02369ac0;
extern u32 data_0208fe70;
extern s32 data_ov000_02369b18[];
extern u8 data_ov000_0236554c[];
extern u8 data_ov000_02365550[];
extern char data_ov000_02365554[];
extern char data_ov000_02365568[];
extern char data_ov000_0236557c[];
extern char data_ov000_02365590[];
extern char data_ov000_023655a4[];
extern char data_ov000_023655b8[];
extern char data_ov000_023655cc[];

UnkTask_022db390 *func_020372d0(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void func_0206c3d0(void);
void func_ov000_022da3f4(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov000_022da24c(void);
u16 func_ov000_022c5f28(void);
void func_02021310(UnkArc_022db390 *ctx, void *a, void *b);
void *func_02021264(UnkArc_022db390 *ctx, u32 id);
void func_020212e0(UnkArc_022db390 *ctx);
void *func_0202128c(const char *path);
void func_020257f8(void *p);
void func_ov000_022fba50(UnkStruct_022db390 *s, u8 *a, s32 x, s32 y);
void func_ov000_022da3b4(void);
void func_02031c1c(void *p, s32 a);
void func_02031bd0(void *p, s32 a, s32 b);
void func_02031a68(void *p, s32 a, void *b, s32 c, s32 d);
void *func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022dae28(s32 k, BOOL f);
void func_ov000_022dad70(s32 k, BOOL f);
void func_020446b0(s32 a);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

UnkStruct_022db390 *func_ov000_022db390(u8 *a, s32 x, s32 y) {
    UnkArc_022db390 arc1;
    UnkArc_022db390 arc2;
    UnkStruct_022db390 *s;
    UnkStruct_022db390_a *p;
    if (a[0] == 0xff && a[1] == 0xff)
        return NULL;
    s = func_020372d0(func_0206c3d0, func_ov000_022da3f4, 0, 0x500, 2, 0x374)->unk10;
    MI_CpuFill16(s, 0, 0x374);
    data_ov000_02369b14 = s;
    s->unk68 = func_ov000_022da24c;
    MI_CpuFill16(&data_ov000_02369ac0, 0, 0xc);
    data_ov000_02369ac0.unk8 = func_ov000_022c5f28();
    data_ov000_02369ac0.unkA = func_ov000_022c5f28();
    func_02021310(&arc1, data_ov000_0236554c, data_022c4488);
    s->unk230 = func_02021264(&arc1, 8);
    s->unk234 = func_02021264(&arc1, 9);
    func_020212e0(&arc1);
    s->unk22C = data_ov000_02369b18[0];
    s->unk238 = data_ov000_02369b18[4];
    func_ov000_022fba50(s, a, x, y);
    s->unk70 = func_ov000_022da3b4;
    s->unk54 |= 0x12;
    s->unk58 |= 0xa300;
    s->unk5C |= 0x20;
    s->unk21E = Rand() & 3;
    s->unk10 = 0;
    s->unk14 = 0x20000;
    s->unk18 = 0;
    p = &s->unk23C;
    func_02031c1c(p, 0);
    func_02031bd0(p, s->unk22C, 0);
    func_02031a68(p, 3, s->unk234, 0, 0);
    p->unk10F |= 4;
    p->unk48 = s->unk10 << 4;
    p->unk4C = -(s->unk14 << 4);
    p->unk50 = s->unk18 << 4;
    p->unk20 = 0x34cc;
    p->unk1C = p->unk20;
    p->unk18 = p->unk1C;
    s->unk214 = func_ov000_022fad3c(0xcc, x, 0, 0, 0, 0, 0, 0, 0);
    func_ov000_022dae28(data_022c4560, 0);
    func_ov000_022dad70(data_022c4560, 0);
    func_02021310(&arc2, data_ov000_02365550, data_022c4488);
    func_020257f8(func_0202128c(data_ov000_02365554));
    func_020257f8(func_0202128c(data_ov000_02365568));
    func_020257f8(func_0202128c(data_ov000_0236557c));
    func_020257f8(func_0202128c(data_ov000_02365590));
    func_020257f8(func_0202128c(data_ov000_023655a4));
    func_020257f8(func_0202128c(data_ov000_023655b8));
    func_020257f8(func_0202128c(data_ov000_023655cc));
    func_020212e0(&arc2);
    func_020446b0(0xb);
    return s;
}
