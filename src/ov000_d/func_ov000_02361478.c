#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0xC - 4];
    s32 unkC;
} UnkStruct_02361478;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[4];
    UnkStruct_02361478 *work;
} UnkStruct_02361478_Task;

extern UnkStruct_02361478_Task *gTaskCurrent;

void *func_020514e4(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MsgBox_Destroy(void);
void Task_DestroyByTag(s32 a);
BOOL func_020513a8(void);
void Wireless_EnableErrorCallback(s32 a);
void StageStart_Create(s32 a);
void SysSound_Release(void);
void Game_StartVsStage(u8 a, u8 b);
void func_ov000_023611f8(UnkStruct_02361478 *w);
void func_ov000_0236119c(UnkStruct_02361478 *w, s32 a, s32 b);

#pragma thumb on

void func_ov000_02361478(void) {
    UnkStruct_02361478 *w = gTaskCurrent->work;

    if (w->unk0 == 8) {
        void *s = func_020514e4();
        vu16 zero = 0;
        MIi_CpuClear16(zero, s, 0x40);
    }
    if (w->unk0 >= 0x10) {
        u8 a;
        s32 n;
        u8 b;
        w->unk0 = 0;
        n = w->unkC;
        a = n / 2;
        b = n % 2;
        MsgBox_Destroy();
        Task_DestroyByTag(0x1000);
        if (func_020513a8()) {
            Wireless_EnableErrorCallback(1);
            StageStart_Create(0);
            return;
        }
        SysSound_Release();
        Game_StartVsStage(a, b);
        return;
    }
    func_ov000_023611f8(w);
    func_ov000_0236119c(w, 0x1000, 0);
    w->unk0++;
}

#pragma thumb reset
