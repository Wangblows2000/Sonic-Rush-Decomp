#include <ov000_b/stageobj.h>

#define REG32(a) (*(vu32 *)(a))

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkStruct_02358548_Pt;

typedef struct {
    u32 unk0;
    u8 pad4[0x10 - 4];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x28 - 0x18];
    u8 unk28[0xB94 - 0x28];
    UnkStruct_02358548_Pt pt[5];
} UnkStruct_02358548;

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_02358548_Bright;

typedef struct {
    u8 pad0[8];
    void *unk8;
    u8 padC[4];
    void *work;
} UnkStruct_02358548_Task;

extern UnkStruct_02358548_Task *gTaskCurrent;
typedef struct {
    u8 pad0[8];
    u16 unk8;
} UnkStruct_02358548_Cnt;

extern UnkStruct_02358548_Cnt gDisplaySub;
extern UnkStruct_02358548_Bright data_02087d44;
extern UnkStruct_02358548_Bright data_0208926c;

void PlayerSelectMenu_AnimateModels(void *p);
void PlayerSelectMenu_DrawModels(void *p);
void func_ov000_02357ec8(UnkStruct_02358548 *w, s32 a, s32 b);
void func_ov000_023582e8(UnkStruct_02358548 *w, s32 a, s32 b);
void func_ov000_023579fc(UnkStruct_02358548 *w, s32 x, s32 y);
void func_ov000_023579c8(UnkStruct_02358548 *w, s32 x, s32 y);
void func_ov000_0235793c(UnkStruct_02358548 *w, s32 x, s32 y);
void func_ov000_02357908(UnkStruct_02358548 *w, s32 x, s32 y);
void func_ov000_02357994(UnkStruct_02358548 *w, s32 x, s32 y);
void func_ov000_02358384(UnkStruct_02358548 *w, s32 a, s32 b);
void func_ov000_02357878(UnkStruct_02358548 *w, s32 a, s32 b, s32 c);
void func_ov000_02358224(UnkStruct_02358548 *w, s32 a, s32 b);
void func_ov000_023580bc(UnkStruct_02358548 *w, s32 a);
void MsgBox_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void PlayerSelectMenu_State_WaitPartnerLeftMsg(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on

void PlayerSelectMenu_State_PartnerLeft(void) {
    UnkStruct_02358548 *w = gTaskCurrent->work;
    UnkStruct_02358548_Pt *p;
    u32 step;
    s32 i;
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;

    PlayerSelectMenu_AnimateModels(w->unk28);
    PlayerSelectMenu_DrawModels(w->unk28);
    func_ov000_02357ec8(w, w->unk14, 0x1000);
    func_ov000_023582e8(w, w->unk14, 0);
    p = &w->pt[0];
    func_ov000_023579fc(w, p->x, p->y);
    p = &w->pt[1];
    func_ov000_023579c8(w, p->x, p->y);
    p = &w->pt[2];
    func_ov000_0235793c(w, p->x, p->y);
    p = &w->pt[3];
    func_ov000_02357908(w, p->x, p->y);
    p = &w->pt[4];
    func_ov000_02357994(w, p->x, p->y);
    if (w->unk10 == 2)
        step = 0x8000;
    else
        step = 0x5555;
    for (i = 1; i < w->unk10; i++)
        func_ov000_02358384(w, (w->unk14 + i) % w->unk10, step * i);
    func_ov000_02357878(w, 0x80, 0x98, 0x1000);
    func_ov000_02358224(w, 0, 0x1000);
    gDisplaySub.unk8++;
    func_ov000_023580bc(w, 1);
    w->unk0++;
    data_02087d44.unk8 = -w->unk0;
    data_0208926c.unk8 = data_02087d44.unk8;
    if (w->unk0 >= 0x10) {
        w->unk0 = 0;
        z0 = 0;
        MIi_CpuClear32(z0, (void *)0x6000000, 0x80000);
        z1 = 0;
        MIi_CpuClear32(z1, (void *)0x6200000, 0x20000);
        z2 = 0;
        MIi_CpuClear32(z2, (void *)0x5000000, 0x200);
        z3 = 0;
        MIi_CpuClear32(z3, (void *)0x5000400, 0x200);
        REG32(0x4000000) = (REG32(0x4000000) & ~0x1f00) | 0x1000;
        REG32(0x4001000) = REG32(0x4001000) & ~0x1f00;
        MsgBox_Create(2, 3, 2, 0, 1, 0, 0xc, 0x100010);
        gTaskCurrent->unk8 = PlayerSelectMenu_State_WaitPartnerLeftMsg;
    }
}

#pragma thumb reset
