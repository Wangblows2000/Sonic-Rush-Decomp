#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 flags;
    u32 pad1c;
    s32 unk20;
    s32 unk24;
} UnkState_0204b95c;

typedef struct {
    u8 pad0[0x14];
    u8 unk14;
} UnkState2_0204b95c;

typedef struct {
    u8 pad0[0x350];
    s16 unk350;
    u8 pad352[0x5c];
    s16 unk3ae;
} UnkPlayer_0204b95c;

typedef struct {
    u8 pad0[0x18];
    s16 unk18;
    s16 unk1a;
    u8 pad1c[0x60];
    s16 unk7c;
    s16 unk7e;
} UnkGfx_0204b95c;

extern UnkState2_0204b95c data_022c4590;
extern UnkState_0204b95c data_022c4560;
extern UnkPlayer_0204b95c *data_ov000_0236ac60[];
extern u32 data_022c4ef0;
extern UnkGfx_0204b95c data_ov000_0236aad8;
extern u16 data_022503e0[];

void func_ov000_023375f8(u8 a);
void func_ov000_02337634(s32 a);
void func_ov000_02337618(s32 a);
void func_ov000_02336050(u16 a, s32 b);
s32 func_02044690(void);
s32 func_02044670(void);
void func_0206c784(s32 a, s32 b);
void func_0206ded8(void);
void func_02067cc8(void);
void func_02055918(s32 mode);
void func_0206c970(void);

void func_0204b95c(void) {
    func_ov000_023375f8(data_022c4590.unk14);
    func_ov000_02337634(data_ov000_0236ac60[0]->unk3ae);
    func_ov000_02337618(data_022c4560.unk20);
    if (data_022c4560.flags & 1)
        data_022c4560.unk20++;
    data_022c4560.unk24++;
    func_ov000_02336050((u16)(data_ov000_0236ac60[0]->unk350 >> 4), 0);
    if ((data_022c4ef0 & 0x80) == 0) {
        func_0206c784(func_02044690(), func_02044670());
        data_ov000_0236aad8.unk18 = -func_02044690();
        data_ov000_0236aad8.unk1a = -func_02044670();
        data_ov000_0236aad8.unk7c = data_ov000_0236aad8.unk18;
        data_ov000_0236aad8.unk7e = data_ov000_0236aad8.unk1a;
    }
    func_0206ded8();
    func_02067cc8();
    if (data_022503e0[2] & 8)
        func_02055918(0);
    if (data_022c4560.flags & 0x20)
        func_0206c970();
}
