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

void Camera_InitBoundsAndBuffers(void);
void Camera_InitFixedVram(void);
void Camera_InitSingleScreen(void);
void Camera_InitDualScreen(void);
void Camera_Main(void);
void Camera_Main_Fixed(void);
void Camera_Destructor(void);
void Camera_Look_Idle(void);
void ZoneFx_Init(void);
void ZoneBg_Init(void);
void Camera_ApplyLcdSwap(void);
void func_ov000_02338778(void);

void Camera_Create(void) {
    UnkStruct_0233c100_Work *w;
    u8 mode;
    vu16 zero0;

    mode = gGameState.unk1C;
    zero0 = 0;
    MIi_CpuClear16(zero0, &gCamera, sizeof(UnkStruct_0236aad8));
    Camera_InitBoundsAndBuffers();
    if (gGameState.unk1C >= 9 || gGameState.unk1D >= 2) {
        data_ov000_0236aab0 |= 4;
        Camera_InitFixedVram();
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
        Camera_InitSingleScreen();
    } else {
        Camera_InitDualScreen();
    }
    if (!(data_ov000_0236aab0 & 4)) {
        vu16 zero1;
        data_ov000_0236aab4 = Task_Create(Camera_Main, Camera_Destructor, 0, 0x80, 3, sizeof(UnkStruct_0233c100_Work));
        w = data_ov000_0236aab4->work;
        zero1 = 0;
        MIi_CpuClear16(zero1, w, sizeof(UnkStruct_0233c100_Work));
        w->unkC = Camera_Look_Idle;
    } else {
        vu16 zero2;
        data_ov000_0236aab4 = Task_Create(Camera_Main_Fixed, Camera_Destructor, 0, 0x80, 3, sizeof(UnkStruct_0233c100_Work));
        w = data_ov000_0236aab4->work;
        zero2 = 0;
        MIi_CpuClear16(zero2, w, sizeof(UnkStruct_0233c100_Work));
        w->unkC = NULL;
    }
    ZoneFx_Init();
    ZoneBg_Init();
    Task_Create(Camera_ApplyLcdSwap, func_ov000_02338778, 0, 0xf00a, 3, 0);
}
