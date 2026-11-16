#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkArc_022d4d28;

typedef struct UnkStruct_022d4d28_a {
    u8 pad[0x138];
} UnkStruct_022d4d28_a;

typedef struct UnkStruct_022d4d28_b {
    u8 pad[0x210];
    s32 unk210;
    UnkStruct_022d4d28_a unk214[11];
    u8 pad1[0x1450 - 0xf7c];
    s32 unk1450;
    s32 unk1454;
    s32 unk1458;
} UnkStruct_022d4d28_b;

typedef struct UnkStruct_022d4d28 {
    u8 pad[0x10];
    UnkStruct_022d4d28_b *unk10;
} UnkStruct_022d4d28;

extern void *data_022c4488;
extern char data_ov000_0236528c[];
extern char data_ov000_02365290[];
extern char data_ov000_023652a8[];
extern char data_ov000_023652c0[];
extern char data_ov000_023652d8[];
extern char data_ov000_023652f0[];
extern char data_ov000_02365308[];
extern char data_ov000_0236531c[];
extern char data_ov000_02365334[];
extern UnkStruct_022d4d28_b *data_ov000_02369ae0;

void func_02031768(UnkStruct_022d4d28_a *a);
BOOL func_02021310(UnkArc_022d4d28 *arc, const char *name, void *narc);
void *func_0202128c(const char *path);
void func_0202566c(void *p);
BOOL func_020212e0(UnkArc_022d4d28 *arc);
void func_020371c8(s32 a);
void func_ov000_022fb9c8(UnkStruct_022d4d28 *s);
void func_020446b0(s32 a);
void func_020365bc(s32 a);

void func_ov000_022d4d28(UnkStruct_022d4d28 *t) {
    UnkStruct_022d4d28_b *s = t->unk10;
    UnkArc_022d4d28 arc;
    s32 i;
    for (i = 0; i < 11; i++)
        func_02031768(&s->unk214[i]);
    func_02021310(&arc, data_ov000_0236528c, data_022c4488);
    func_0202566c(func_0202128c(data_ov000_02365290));
    func_0202566c(func_0202128c(data_ov000_023652a8));
    func_0202566c(func_0202128c(data_ov000_023652c0));
    func_0202566c(func_0202128c(data_ov000_023652d8));
    func_0202566c(func_0202128c(data_ov000_023652f0));
    func_0202566c(func_0202128c(data_ov000_02365308));
    func_0202566c(func_0202128c(data_ov000_0236531c));
    func_0202566c(func_0202128c(data_ov000_02365334));
    func_020212e0(&arc);
    func_020371c8(s->unk210);
    func_ov000_022fb9c8(t);
    func_020446b0(0);
    func_020365bc(s->unk1450);
    func_020365bc(s->unk1454);
    func_020365bc(s->unk1458);
    data_ov000_02369ae0 = NULL;
}
