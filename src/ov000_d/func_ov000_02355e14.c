#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad0C[0x1C - 0xC];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

void SysSound_Release(void);
void SceneChange_ToZoneMap(void);
void func_0205240c(void);
void func_ov000_02362b40(void);
void func_ov000_02362b28(void);
u8 func_0204e6b0(s32 a);
void SceneChange_ToTimeAttackMenuMode0(void);
void SceneChange_ToTimeAttackMenuMode1(void);

#pragma thumb on

void func_ov000_02355e14(s32 mode, s32 sub) {
    switch (sub) {
    case 0:
        gGameState.unk0 = 0;
        break;
    case 1:
        gGameState.unk0 = 1;
        break;
    case 2:
        gGameState.unk0 = 0;
        break;
    }
    switch (mode) {
    case 0:
        switch (sub) {
        case 0:
        case 1:
            SysSound_Release();
            SceneChange_ToZoneMap();
            break;
        case 2:
            SysSound_Release();
            func_0205240c();
            break;
        }
        break;
    case 1:
        switch (sub) {
        case 0:
            gGameState.unk4 = 0;
            gGameState.unk8 = 1;
            break;
        case 1:
            gGameState.unk4 = 1;
            gGameState.unk8 = 0;
            break;
        }
        func_ov000_02362b40();
        break;
    case 2:
        switch (sub) {
        case 0:
            gGameState.unk4 = 1;
            gGameState.unk8 = 0;
            break;
        case 1:
            gGameState.unk4 = 0;
            gGameState.unk8 = 1;
            break;
        }
        func_ov000_02362b28();
        break;
    case 3:
        gGameState.unk1C = func_0204e6b0(0);
        gGameState.unk1D = 0;
        SceneChange_ToTimeAttackMenuMode0();
        break;
    case 4:
        gGameState.unk1C = func_0204e6b0(0);
        gGameState.unk1D = 0;
        SceneChange_ToTimeAttackMenuMode1();
        break;
    }
}

#pragma thumb reset
