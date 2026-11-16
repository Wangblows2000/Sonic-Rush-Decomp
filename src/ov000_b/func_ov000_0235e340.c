#include <ov000_b/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[4];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x20 - 0x10];
    s32 unk20;
    u8 pad24[0xC14 - 0x24];
    u8 unkC14[4];
} UnkStruct_0235e340;

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_0235e340_Bright;

extern UnkStruct_Task *gTaskCurrent;
extern UnkStruct_0235e340_Bright data_0208926c;
extern UnkStruct_0235e340_Bright data_02087d44;

void func_ov000_0235e124(UnkStruct_0235e340 *w);
void func_ov000_0235e068(UnkStruct_0235e340 *w);
void func_ov000_0235e018(UnkStruct_0235e340 *w);
void func_ov000_0235dfc4(UnkStruct_0235e340 *w);
void func_ov000_0235def0(UnkStruct_0235e340 *w, s32 a, s32 b);
s32 func_ov000_0235c9f8(s32 a, s32 b, s32 c, s32 d, s32 e);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void func_ov000_0235cb84(s32 a, void *b);
void func_0205a4ec(s32 a);
void Task_DestroyAll(void);
BOOL func_ov000_0235cbd8(void);
void func_ov000_0235caf8(s32 a);
void WirelessSession_Stop(void);
void func_ov000_0235e1c4(void);

#pragma thumb on

void func_ov000_0235e340(void) {
    UnkStruct_0235e340 *w = gTaskCurrent->work;
    s32 t;
    s32 target;
    s32 next;
    s16 b;

    func_ov000_0235e124(w);
    func_ov000_0235e068(w);
    func_ov000_0235e018(w);
    func_ov000_0235dfc4(w);
    func_ov000_0235def0(w, w->unk20, 0x1000);
    t = w->unk0;
    if (w->unkC != 0)
        target = -0x10;
    else
        target = 0x10;
    if (t >= 0) {
        b = func_ov000_0235c9f8(0, target, 0x10, t, 0x1000);
        data_0208926c.unk8 = b;
        data_02087d44.unk8 = b;
    }
    w->unk0++;
    if (w->unk0 >= 0x10) {
        w->unk0 = 0;
        next = w->unk8;
        b = target;
        data_0208926c.unk8 = b;
        data_02087d44.unk8 = b;
        GXx_SetMasterBrightness_((vu16 *)0x400006c, b);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, b);
        func_ov000_0235cb84(w->unk20, w->unkC14);
        func_0205a4ec(0);
        Task_DestroyAll();
        if (func_ov000_0235cbd8()) {
            func_ov000_0235caf8(next);
            return;
        }
        if (next == 2) {
            WirelessSession_Stop();
            func_ov000_0235e1c4();
            return;
        }
        func_ov000_0235caf8(next);
    }
}

#pragma thumb reset
