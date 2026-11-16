#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad0C[0x1C - 0xC];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

void SysSound_Release(void);
void SceneChange_Unk_02054fd4(void);
void func_0205240c(void);
void func_ov000_02362b40(void);
void func_ov000_02362b28(void);
u8 func_0204e6b0(s32 a);
void SceneChange_Unk_02054f9c(void);
void SceneChange_Unk_02054f80(void);

#pragma thumb on

void func_ov000_02355e14(s32 mode, s32 sub) {
    switch (sub) {
    case 0:
        data_022c4560.unk0 = 0;
        break;
    case 1:
        data_022c4560.unk0 = 1;
        break;
    case 2:
        data_022c4560.unk0 = 0;
        break;
    }
    switch (mode) {
    case 0:
        switch (sub) {
        case 0:
        case 1:
            SysSound_Release();
            SceneChange_Unk_02054fd4();
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
            data_022c4560.unk4 = 0;
            data_022c4560.unk8 = 1;
            break;
        case 1:
            data_022c4560.unk4 = 1;
            data_022c4560.unk8 = 0;
            break;
        }
        func_ov000_02362b40();
        break;
    case 2:
        switch (sub) {
        case 0:
            data_022c4560.unk4 = 1;
            data_022c4560.unk8 = 0;
            break;
        case 1:
            data_022c4560.unk4 = 0;
            data_022c4560.unk8 = 1;
            break;
        }
        func_ov000_02362b28();
        break;
    case 3:
        data_022c4560.unk1C = func_0204e6b0(0);
        data_022c4560.unk1D = 0;
        SceneChange_Unk_02054f9c();
        break;
    case 4:
        data_022c4560.unk1C = func_0204e6b0(0);
        data_022c4560.unk1D = 0;
        SceneChange_Unk_02054f80();
        break;
    }
}

#pragma thumb reset
