#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0xE0];
    s32 unkE0;
    s32 unkE4;
    u8 padE8[0x140 - 0xE8];
} UnkStruct_0236aad8;

typedef struct {
    u8 pad0[0xC];
    void *unkC;
} UnkStruct_0233c100_Work;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_0236aad8 gCamera;
extern u16 data_ov000_0236aab0;
extern UnkStruct_Task *data_ov000_0236aab4;

void func_ov000_0233a27c(void);
void func_ov000_0233a000(void);
void func_ov000_0233a010(void);
void func_ov000_0233a118(void);
void func_ov000_02339dd4(void);
void func_ov000_02339db8(void);
void func_ov000_02339eec(void);
void func_ov000_02338b54(void);
void func_ov000_0233e79c(void);
void func_ov000_02342068(void);
void func_ov000_02338728(void);
void func_ov000_02338778(void);

void func_ov000_0233c100(void) {
    UnkStruct_0233c100_Work *w;
    u8 mode;
    vu16 zero0;

    mode = gGameState.unk1C;
    zero0 = 0;
    MIi_CpuClear16(zero0, &gCamera, sizeof(UnkStruct_0236aad8));
    func_ov000_0233a27c();
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2) {
        data_ov000_0236aab0 |= 4;
        func_ov000_0233a000();
        if (mode == 3) {
            gCamera.unkE0 = 0x128fb8;
            gCamera.unkE4 = 0x749fed;
        } else if (mode == 2) {
            gCamera.unkE0 = 0xf42f8;
            gCamera.unkE4 = 0x5fe435;
        } else if (mode == 1) {
            gCamera.unkE0 = 0x1152f0;
            gCamera.unkE4 = 0x6cd988;
        }
    } else if (gGameState.unk18 & 0x20) {
        func_ov000_0233a010();
    } else {
        func_ov000_0233a118();
    }
    if (!(data_ov000_0236aab0 & 4)) {
        vu16 zero1;
        data_ov000_0236aab4 = Task_Create(func_ov000_02339dd4, func_ov000_02339eec, 0, 0x80, 3, sizeof(UnkStruct_0233c100_Work));
        w = data_ov000_0236aab4->work;
        zero1 = 0;
        MIi_CpuClear16(zero1, w, sizeof(UnkStruct_0233c100_Work));
        w->unkC = func_ov000_02338b54;
    } else {
        vu16 zero2;
        data_ov000_0236aab4 = Task_Create(func_ov000_02339db8, func_ov000_02339eec, 0, 0x80, 3, sizeof(UnkStruct_0233c100_Work));
        w = data_ov000_0236aab4->work;
        zero2 = 0;
        MIi_CpuClear16(zero2, w, sizeof(UnkStruct_0233c100_Work));
        w->unkC = NULL;
    }
    func_ov000_0233e79c();
    func_ov000_02342068();
    Task_Create(func_ov000_02338728, func_ov000_02338778, 0, 0xf00a, 3, 0);
}
