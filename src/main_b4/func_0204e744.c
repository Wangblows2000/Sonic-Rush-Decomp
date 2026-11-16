#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0xc];
    s32 unk10;
    u8 pad14[8];
    u8 unk1c;
    u8 unk1d;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
} UnkStruct_022c4680;

extern UnkStruct_022c4560 data_022c4560;
extern u16 data_022c45f8;
extern UnkStruct_022c4680 gSaveData;

void func_0204d684(void (*f)(void), s32 a);
void SceneChange_Unk_02054df8(void);
void SceneChange_Unk_02054ef4(void);
void SceneChange_Unk_02054f10(void);
void func_0204e4b8(void);
void func_0204e498(void);
void func_02054378(void);
void NaviB_Release(void);
void func_02048858(void);
void SceneChange_Unk_0205500c(void);
void SceneChange_Unk_02054fb8(void);
void SceneChange_Unk_02054ff0(void);

void func_0204e744(void) {
    u8 mode;

    if (data_022c4560.unk10 == 2) {
        func_0204d684(SceneChange_Unk_02054df8, 3);
        return;
    }
    mode = data_022c4560.unk1c;
    if (mode == 8) {
        SceneChange_Unk_02054ef4();
        return;
    }
    if (mode == 7) {
        func_0204d684(SceneChange_Unk_02054f10, 3);
        return;
    }
    if (data_022c45f8 & 1) {
        data_022c45f8 &= ~1;
        switch (data_022c4560.unk1d) {
        case 0:
            func_0204d684(func_0204e4b8, 3);
            return;
        case 1:
            func_0204d684(func_0204e498, 3);
            return;
        case 2:
            if (mode == 6) {
                func_02054378();
                NaviB_Release();
                func_0204d684(func_02048858, 1);
                return;
            }
            func_02054378();
            NaviB_Release();
            func_0204d684(SceneChange_Unk_0205500c, 1);
            return;
        }
    } else {
        switch (data_022c4560.unk1d) {
        case 0:
        case 1:
            if (data_022c4560.unk0 != 1 && mode == 0 && gSaveData.unkc < 2) {
                func_0204d684(SceneChange_Unk_02054fb8, 3);
                return;
            }
            func_0204d684(SceneChange_Unk_02054ff0, 3);
            return;
        case 2:
            func_02054378();
            NaviB_Release();
            func_0204d684(SceneChange_Unk_02054ff0, 1);
            return;
        }
    }
}
