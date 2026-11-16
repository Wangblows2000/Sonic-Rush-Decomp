#include <nitro/types.h>

typedef struct {
    u8 pad[0x18];
    u32 unk18;
    u8 pad1C[0x138 - 0x1C];
} UnkStruct_Sub;

typedef struct {
    u8 pad[0x78];
    s32 unk78;
} UnkStruct_Obj;

typedef struct UnkStruct_022dd748 UnkStruct_022dd748;
struct UnkStruct_022dd748 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022dd748 *s);
    u8 pad6C[0x78 - 0x6C];
    s32 unk78;
    u8 pad7C[0x20C - 0x7C];
    UnkStruct_Obj *unk20C;
    UnkStruct_Obj *unk210;
    UnkStruct_Sub unk214[7];
};

typedef struct {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad5E[0x2AA - 0x5E];
    u16 unk2AA;
    u8 pad2AC[0x2D6 - 0x2AC];
    u16 unk2D6;
    u8 pad2D8[0x3B2 - 0x2D8];
    u16 unk3B2;
} UnkStruct_Player;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_0208922c;

extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_Player *data_ov000_0236ac60;

void func_ov000_022dd5d8(UnkStruct_022dd748 *s);
void ScreenShake_Start(s32 a);
void func_0201bfbc(s32 a);
void func_ov000_022dcc10(UnkStruct_022dd748 *s);
void func_ov000_022dbc4c(s32 a);
void func_ov000_022dbb7c(s32 a);
void func_ov000_022dba50(s32 a);
void func_ov000_022dbab4(s32 a);
u32 func_ov000_0235b0a8(void);
void func_ov000_0235b08c(u32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022c5c80(s32 a);
void func_ov000_02347c44(UnkStruct_Player *p);
void func_ov000_0234c4a4(UnkStruct_Player *p, s32 a);

void func_ov000_022dd748(UnkStruct_022dd748 *s) {
    UnkStruct_Obj *a = s->unk20C;
    UnkStruct_Obj *b = s->unk210;
    s32 i;

    data_ov000_02369ac0.unk4 = 0x1a;
    s->func = func_ov000_022dd5d8;
    *(s32 *)((u8 *)s + 0x220) = 0;
    *(s32 *)((u8 *)s + 0x224) = 0;
    data_022c4560.unk18 &= ~1;
    ScreenShake_Start(0);
    func_0201bfbc(0);
    func_ov000_022dcc10(s);
    for (i = 0; i < 7; i++)
        s->unk214[i].unk18 |= 2;
    func_ov000_022dbc4c(0);
    func_ov000_022dbb7c(0);
    func_ov000_022dba50(0);
    func_ov000_022dbab4(0);
    func_ov000_0235b08c(func_ov000_0235b0a8() | 1);
    gDisplayMain.unk48 = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7c);
    func_ov000_022c5c80(6);
    func_ov000_02347c44(data_ov000_0236ac60);
    func_ov000_0234c4a4(data_ov000_0236ac60, 0x30);
    data_ov000_0236ac60->unk3B2 = 0;
    data_ov000_0236ac60->unk5C &= ~0x20;
    data_ov000_0236ac60->unk2AA &= ~4;
    data_ov000_0236ac60->unk2AA |= 0x1000;
    data_ov000_0236ac60->unk2D6 &= ~4;
    data_ov000_0236ac60->unk2D6 |= 0x1000;
    s->unk78 = 0;
    a->unk78 = 0;
    b->unk78 = 0;
}
