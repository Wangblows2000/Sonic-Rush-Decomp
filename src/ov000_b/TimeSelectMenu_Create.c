#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[0x28 - 0x10];
    s32 unk28;
    s32 unk2C;
    u8 pad30[0x6A8 - 0x30];
    u8 unk6A8[0x103C - 0x6A8];
} UnkStruct_023629cc;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u16 unk6;
} UnkStruct_023629cc_Pad;

typedef struct {
    u8 pad0[8];
    void *unk8;
    u8 padC[4];
    void *work;
} UnkStruct_023629cc_Task;

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

UnkStruct_023629cc_Task *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void TimeSelectMenu_Main(void);
void TimeSelectMenu_Destructor(void);
void TimeSelectMenu_ScrollBg_Main(void);
void TimeSelectMenu_State_Sync(void);
void TimeSelectMenu_InitDisplay(void);
void TimeSelectMenu_LoadAssets(UnkStruct_023629cc *w);
void TimeSelectMenu_InitSprites(UnkStruct_023629cc *w);
void TimeSelectMenu_InitBackgrounds(UnkStruct_023629cc *w);
void TimeSelectMenu_InitKeyframeAnims(UnkStruct_023629cc *w);
void TimeSelectMenu_LoadArchives(UnkStruct_023629cc *w);
void func_ov000_02361174(UnkStruct_023629cc *w);
void func_ov000_02360f18(UnkStruct_023629cc *w, s32 a);
void func_ov000_02360ee0(UnkStruct_023629cc *w, s32 a, s32 b);
void func_ov000_02360e6c(UnkStruct_023629cc *w);
void func_ov000_02360d74(UnkStruct_023629cc *w);
void func_ov000_02360d3c(UnkStruct_023629cc *w);
void func_ov000_02360c88(UnkStruct_023629cc *w);
void func_ov000_02360bd4(UnkStruct_023629cc *w);
void func_ov000_02360b9c(UnkStruct_023629cc *w);
void func_ov000_02360b64(UnkStruct_023629cc *w);
void func_ov000_02360b2c(UnkStruct_023629cc *w);
void func_ov000_02360af4(UnkStruct_023629cc *w);
void TimeSelectMenu_LoadPlayerNames(UnkStruct_023629cc *w);
UnkStruct_023629cc_Pad *func_020514e4(void);
UnkStruct_023629cc_Pad *func_02051474(void);
void TimeSelectMenu_CreateLinkIcon(void *p);
void Touch_Enable(void);

#pragma thumb on

void TimeSelectMenu_Create(s32 a, s32 b) {
    UnkStruct_023629cc_Task *task;
    UnkStruct_023629cc *w;
    UnkStruct_023629cc_Pad *p;
    vu32 zero;

    task = Task_Create(TimeSelectMenu_Main, TimeSelectMenu_Destructor, 0, 0x1000, 0x1000, sizeof(UnkStruct_023629cc));
    w = task->work;
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkStruct_023629cc));
    w->unk0 = 0;
    if (a == 0) {
        w->unk4 = 1;
        if (b == 0) {
            w->unk28 = 0;
            w->unk2C = 1;
        } else {
            w->unk28 = 1;
            w->unk2C = 0;
        }
    } else {
        w->unk4 = 0;
        if (b == 0) {
            w->unk2C = 0;
            w->unk28 = 1;
        } else {
            w->unk2C = 1;
            w->unk28 = 0;
        }
    }
    w->unkC = 0;
    TimeSelectMenu_InitDisplay();
    TimeSelectMenu_LoadAssets(w);
    TimeSelectMenu_InitSprites(w);
    TimeSelectMenu_InitBackgrounds(w);
    TimeSelectMenu_InitKeyframeAnims(w);
    TimeSelectMenu_LoadArchives(w);
    func_ov000_02361174(w);
    func_ov000_02360f18(w, 0);
    func_ov000_02360ee0(w, 0, 0);
    func_ov000_02360e6c(w);
    func_ov000_02360d74(w);
    func_ov000_02360d3c(w);
    func_ov000_02360c88(w);
    func_ov000_02360bd4(w);
    func_ov000_02360b9c(w);
    func_ov000_02360b64(w);
    func_ov000_02360b2c(w);
    func_ov000_02360af4(w);
    TimeSelectMenu_LoadPlayerNames(w);
    p = func_020514e4();
    p->unk0 = 0;
    p->unk1 = 0;
    p->unk2 = 0;
    p->unk3 = 0;
    p->unk4 = 0;
    p->unk6 = 0;
    p = func_02051474();
    p->unk0 = 0;
    p->unk1 = 0;
    p->unk2 = 0;
    p->unk3 = 0;
    p->unk4 = 0;
    p->unk6 = 0;
    Task_Create(TimeSelectMenu_ScrollBg_Main, NULL, 0, 0x3000, 0x1000, 0);
    gSaveDataBusy = 1;
    gSystemFlags |= 0x10;
    TimeSelectMenu_CreateLinkIcon(w->unk6A8);
    Touch_Enable();
    task->unk8 = TimeSelectMenu_State_Sync;
}

#pragma thumb reset
