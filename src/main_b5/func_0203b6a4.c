#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unka;
    u16 unkc;
} UnkStruct_022b89e8;

extern UnkStruct_022b89e8 data_022b89e8;

s32 func_0203af98(u32 n);
void MB_DisconnectChild(u32 n);
void func_0203b09c(s32 a);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);

void func_0203b6a4(void) {
    u16 i;
    u32 e;
    u16 mask;
    s32 bit;

    func_0203b09c(3);
    for (i = 1; i < 16; i++) {
        bit = 1 << i;
        if ((data_022b89e8.unk2 & bit) && func_0203af98(i) == 1) {
            mask = ~bit;
            e = OS_DisableInterrupts();
            data_022b89e8.unk2 &= mask;
            data_022b89e8.unk4 &= mask;
            data_022b89e8.unk6 &= mask;
            data_022b89e8.unk8 &= mask;
            data_022b89e8.unka &= mask;
            data_022b89e8.unkc &= mask;
            OS_RestoreInterrupts(e);
            MB_DisconnectChild(i);
        }
    }
}
