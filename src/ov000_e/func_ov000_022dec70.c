#include <nitro/types.h>

typedef struct {
    u8 data[0x138];
} UnkStruct_022dec70_M;

typedef struct {
    u8 pad[0x228];
    UnkStruct_022dec70_M mdl[7];
    u8 padA90[0xABC - 0x228 - 7 * 0x138];
    void *unkABC;
    void *unkAC0;
    u8 padAC4[0xAD4 - 0xAC4];
    void *unkAD4;
    u8 padAD8[0xAE0 - 0xAD8];
    void *unkAE0;
    u8 padAE4[4];
    void *unkAE8;
} UnkStruct_022dec70_B;

typedef struct { u8 data[0x5c]; } UnkArc_022dec70;

extern void *data_022c4488;
extern char data_ov000_02365728[];
extern char data_ov000_0236572c[];
extern char data_ov000_02365740[];
extern char data_ov000_0236575c[];
extern UnkStruct_022dec70_B *data_ov000_02369b2c;

BOOL func_02021310(UnkArc_022dec70 *arc, const char *name, void *narc);
void *func_0202128c(const char *path);
void func_0202566c(void *p);
BOOL func_020212e0(UnkArc_022dec70 *arc);
void func_02031768(void *a);
void func_ov000_022fb9c8(void *t);

void func_ov000_022dec70(void *t) {
    UnkArc_022dec70 arc;
    UnkStruct_022dec70_B *s = data_ov000_02369b2c;
    s32 i;

    for (i = 0; i < 7; i++)
        func_02031768(&s->mdl[i]);
    func_02021310(&arc, data_ov000_02365728, data_022c4488);
    func_0202566c(func_0202128c(data_ov000_0236572c));
    func_0202566c(func_0202128c(data_ov000_02365740));
    func_0202566c(func_0202128c(data_ov000_0236575c));
    func_020212e0(&arc);
    func_0202566c(s->unkABC);
    func_0202566c(s->unkAC0);
    func_0202566c(s->unkAD4);
    func_0202566c(s->unkAE0);
    func_0202566c(s->unkAE8);
    func_ov000_022fb9c8(t);
    data_ov000_02369b2c = NULL;
}
