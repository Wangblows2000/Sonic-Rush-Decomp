/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gSaveData[];
extern u8 data_022c4560[];
s32 func_0204ec40();
s32 func_0206488c();
s32 SysSound_Release();
s32 func_0205247c();
s32 SceneChange_Unk_02054fb8();
s32 SceneChange_Unk_02054fd4();
s32 SceneChange_Unk_02055098();

void func_ov001_022d6eac(void) {
    s32 t0 = *(u16 *)((u8 *)gSaveData + 0xc);
    *(u32 *)data_022c4560 = 0;
    func_0204ec40(data_022c4560, 0);
    func_0206488c();
    if (t0 == 0) {
        SysSound_Release();
        *(u32 *)data_022c4560 = 0;
        func_0205247c(0, 0);
        return;
    }
    if (t0 == 1) {
        SceneChange_Unk_02054fb8();
        return;
    }
    if (t0 == 2) {
        SysSound_Release();
        *(u32 *)data_022c4560 = 0;
        *(u32 *)((u8 *)data_022c4560 + 0x10) = 0;
        s32 t1 = (*(u32 *)((u8 *)data_022c4560 + 0x18) & ~0x20);
        *(u32 *)((u8 *)data_022c4560 + 0x18) = t1;
        *(u32 *)((u8 *)data_022c4560 + 0x18) = (t1 & ~0x10);
        SceneChange_Unk_02054fd4(0x10, data_022c4560, (t1 & ~0x10));
        return;
    }
    *(u32 *)((u8 *)data_022c4560 + 0x10) = 0;
    s32 t2 = (*(u32 *)((u8 *)data_022c4560 + 0x18) & ~0x20);
    *(u32 *)((u8 *)data_022c4560 + 0x18) = t2;
    *(u32 *)((u8 *)data_022c4560 + 0x18) = (t2 & ~0x10);
    SceneChange_Unk_02055098(0x10, data_022c4560, (t2 & ~0x10));
}

#pragma thumb reset
