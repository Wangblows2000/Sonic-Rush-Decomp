#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x18 - 4];
    s32 unk18;
    s32 unk1C;
} UnkStruct_0236157c;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[4];
    UnkStruct_0236157c *work;
} UnkStruct_0236157c_Task;

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_0236157c_Pad;

typedef struct {
    u8 pad0[0x10];
    u32 flags;
} UnkStruct_0236157c_Touch;

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_0236157c_Sync;

extern UnkStruct_0236157c_Task *data_022b4574;
extern UnkStruct_0236157c_Pad data_022503e0;
extern UnkStruct_0236157c_Touch data_022b65b4;

void func_ov000_023611f8(UnkStruct_0236157c *w);
void func_ov000_0236119c(UnkStruct_0236157c *w, s32 a, s32 b);
UnkStruct_0236157c_Sync *func_020514e4(void);
BOOL func_ov000_0235fb50(UnkStruct_0236157c *w);
void func_02051628(s32 a);
void func_ov000_0235f850(s32 x, s32 y, s32 a, s32 b);
void func_ov000_02361478(void);

#pragma thumb on

void func_ov000_0236157c(void) {
    UnkStruct_0236157c *w = data_022b4574->work;

    func_ov000_023611f8(w);
    func_ov000_0236119c(w, 0x1000, 0);
    w->unk0++;
    if ((data_022503e0.trg & 1) || (data_022b65b4.flags & 0x40)) {
        w->unk1C = 1;
        func_020514e4()->unk3 = 1;
    }
    if (func_ov000_0235fb50(w)) {
        w->unk18 = 1;
    }
    if ((w->unk18 && w->unk1C) || w->unk0 >= 0xc8) {
        w->unk0 = 0;
        func_02051628(0);
        func_ov000_0235f850(0, 0x10, 0x10, 0);
        data_022b4574->unk8 = func_ov000_02361478;
    }
}

#pragma thumb reset
