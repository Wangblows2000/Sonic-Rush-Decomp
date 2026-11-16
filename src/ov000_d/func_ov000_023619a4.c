#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x18 - 4];
    s32 unk18;
    s32 unk1C;
} UnkStruct_023619a4;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[4];
    UnkStruct_023619a4 *work;
} UnkStruct_023619a4_Task;

typedef struct {
    u16 cont;
    u8 pad2[2];
    u16 trg;
} UnkStruct_023619a4_Pad;

typedef struct {
    u8 pad0[0x10];
    u32 flags;
} UnkStruct_023619a4_Touch;

typedef struct {
    u8 pad0[3];
    u8 unk3;
} UnkStruct_023619a4_Sync;

extern UnkStruct_023619a4_Task *gTaskCurrent;
extern UnkStruct_023619a4_Pad gPad;
extern UnkStruct_023619a4_Touch gTouchState;

void func_ov000_023611f8(UnkStruct_023619a4 *w);
void func_ov000_0236119c(UnkStruct_023619a4 *w, s32 a, s32 b);
UnkStruct_023619a4_Sync *func_020514e4(void);
BOOL func_ov000_0235fb24(UnkStruct_023619a4 *w);
void Wireless_EnableErrorCallback(s32 a);
void func_ov000_0235f850(s32 x, s32 y, s32 a, s32 b);
void func_ov000_02361478(void);

#pragma thumb on

void func_ov000_023619a4(void) {
    UnkStruct_023619a4 *w = gTaskCurrent->work;

    func_ov000_023611f8(w);
    func_ov000_0236119c(w, 0x1000, 0);
    w->unk0++;
    if ((gPad.trg & 1) || (gTouchState.flags & 0x40)) {
        w->unk18 = 1;
        func_020514e4()->unk3 = 1;
    }
    if (func_ov000_0235fb24(w)) {
        w->unk1C = 1;
    }
    if ((w->unk18 && w->unk1C) || w->unk0 >= 0xc8) {
        w->unk0 = 0;
        Wireless_EnableErrorCallback(0);
        func_ov000_0235f850(0, 0x10, 0x10, 0);
        gTaskCurrent->unk8 = func_ov000_02361478;
    }
}

#pragma thumb reset
