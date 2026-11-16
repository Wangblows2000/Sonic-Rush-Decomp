#include <ov000_c/stageobj.h>

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
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x3b2 - 0x20c];
    u16 unk3B2;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x358 - 0x210];
    s32 unk358;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x584 - 0x20C];
    s32 unk584;
} UnkStruct_022ef7b0;

typedef struct {
    u32 unk0;
    u8 pad4[0x18 - 4];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_Boss *data_ov000_02369b84;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Player *data_ov000_0236ac60;
extern UnkStruct_0208922c data_0208922c;

void func_ov000_022f9044(UnkStruct_022ef7b0 *s);
void func_ov000_022f4cbc(UnkStruct_022ef7b0 *s);
void func_0206c7d8(void);
void func_ov000_022f8b70(u32 a, s32 b);
void func_ov000_02347c44(UnkStruct_Player *p);
void func_ov000_0234c4a4(UnkStruct_Player *p, s32 a);
u32 func_ov000_0235b0a8(void);
void func_ov000_0235b08c(u32 a);
void func_02036fdc(s32 a);
void func_0201bfbc(s32 a);
void func_ov000_022ef6a0(void);

void func_ov000_022ef7b0(UnkStruct_022ef7b0 *s) {
    UnkStruct_Boss *b = data_ov000_02369b84;

    data_ov000_02369ac0.unk4 = 0x3b;
    func_ov000_022f9044(s);
    func_ov000_022f4cbc(s);
    s->base.unk1B8 = NULL;
    b->base.unk54 |= 0x20;
    s->base.unk54 |= 0x20;
    b->unk358 = 0;
    s->unk584 = 0;
    func_0206c7d8();
    func_ov000_022f8b70(data_022c4560.unk0, 0);
    data_022c4560.unk18 &= ~1;
    func_ov000_02347c44(data_ov000_0236ac60);
    func_ov000_0234c4a4(data_ov000_0236ac60, 0x30);
    data_ov000_0236ac60->unk3B2 = 0;
    data_ov000_0236ac60->base.unk10 = 0x19e00;
    data_ov000_0236ac60->base.unk14 = 0x2b100;
    data_ov000_0236ac60->base.unk18 = 0x2000;
    s->base.unk18 += 0x1400;
    func_ov000_0235b08c(func_ov000_0235b0a8() | 1);
    data_ov000_02369b84->unk20C &= ~0xe;
    func_02036fdc(4);
    data_0208922c.unk20.bits.b6 = 0;
    b->unk20C &= ~0x10;
    func_0201bfbc(0);
    s->base.unk68 = func_ov000_022ef6a0;
}
