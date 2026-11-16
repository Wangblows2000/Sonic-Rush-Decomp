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

BOOL MB_CommResponseRequest(u32 n, s32 a);
void MB_DisconnectChild(u32 n);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);

void func_0203b75c(u32 n) {
    u32 e;
    u16 mask;

    if (!MB_CommResponseRequest(n, 2)) {
        mask = ~(1 << n);
        e = OS_DisableInterrupts();
        data_022b89e8.unk2 &= mask;
        data_022b89e8.unk4 &= mask;
        data_022b89e8.unk6 &= mask;
        data_022b89e8.unk8 &= mask;
        data_022b89e8.unka &= mask;
        data_022b89e8.unkc &= mask;
        OS_RestoreInterrupts(e);
        MB_DisconnectChild(n);
    } else {
        e = OS_DisableInterrupts();
        data_022b89e8.unk6 &= ~(1 << n);
        data_022b89e8.unk8 |= 1 << n;
        OS_RestoreInterrupts(e);
    }
}
