#include <nitro/types.h>

typedef struct UnkStruct_022edee8_b {
    u8 pad[0x24c];
    u8 unk24C[0x388 - 0x24c];
    s32 unk388;
} UnkStruct_022edee8_b;

typedef struct UnkStruct_022edee8 {
    u8 pad[0x10];
    UnkStruct_022edee8_b *unk10;
} UnkStruct_022edee8;

typedef struct { u8 data[0x5c]; } UnkArc_022edee8;

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 1;
            u16 b1 : 5;
            u16 b6 : 2;
            u16 b8 : 8;
        } bits;
    } unk20;
} UnkStruct_02087d04;

extern void *data_022c4488;
extern UnkStruct_02087d04 data_02087d04;
extern char data_ov000_023659c4[];
extern char data_ov000_023659c8[];
extern char data_ov000_023659dc[];
extern char data_ov000_023659f4[];
extern char data_ov000_02365a08[];
extern char data_ov000_02365a20[];
extern char data_ov000_02365a38[];
extern char data_ov000_02365a50[];
extern char data_ov000_02365a64[];
extern char data_ov000_02365a80[];
extern char data_ov000_02365a9c[];
extern char data_ov000_02365ab8[];
extern char data_ov000_02365acc[];

BOOL func_02021310(UnkArc_022edee8 *arc, const char *name, void *narc);
void *func_0202128c(const char *path);
void func_0202566c(void *p);
BOOL func_020212e0(UnkArc_022edee8 *arc);
void func_020365bc(s32 a);
void func_02031768(void *a);
void func_ov000_022fb9c8(UnkStruct_022edee8 *t);

void func_ov000_022edee8(UnkStruct_022edee8 *t) {
    UnkArc_022edee8 arc;
    UnkStruct_022edee8_b *s = t->unk10;
    func_02021310(&arc, data_ov000_023659c4, data_022c4488);
    func_0202566c(func_0202128c(data_ov000_023659c8));
    func_0202566c(func_0202128c(data_ov000_023659dc));
    func_0202566c(func_0202128c(data_ov000_023659f4));
    func_0202566c(func_0202128c(data_ov000_02365a08));
    func_0202566c(func_0202128c(data_ov000_02365a20));
    func_0202566c(func_0202128c(data_ov000_02365a38));
    func_0202566c(func_0202128c(data_ov000_02365a50));
    func_0202566c(func_0202128c(data_ov000_02365a64));
    func_0202566c(func_0202128c(data_ov000_02365a80));
    func_0202566c(func_0202128c(data_ov000_02365a9c));
    func_0202566c(func_0202128c(data_ov000_02365ab8));
    func_0202566c(func_0202128c(data_ov000_02365acc));
    func_020212e0(&arc);
    data_02087d04.unk20.bits.b6 = 0;
    func_020365bc(s->unk388);
    func_02031768(s->unk24C);
    func_ov000_022fb9c8(t);
}
