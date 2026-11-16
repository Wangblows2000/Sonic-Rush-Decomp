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

BOOL func_02016978(u32 n, s32 a);
void func_0201854c(u32 n);
u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);

void func_0203b828(u32 n) {
    u32 e;
    u16 mask;

    if (!func_02016978(n, 0)) {
        mask = ~(1 << n);
        e = OS_DisableInterrupts();
        data_022b89e8.unk2 &= mask;
        data_022b89e8.unk4 &= mask;
        data_022b89e8.unk6 &= mask;
        data_022b89e8.unk8 &= mask;
        data_022b89e8.unka &= mask;
        data_022b89e8.unkc &= mask;
        OS_RestoreInterrupts(e);
        func_0201854c(n);
    } else {
        e = OS_DisableInterrupts();
        data_022b89e8.unk4 &= ~(1 << n);
        data_022b89e8.unk2 &= ~(1 << n);
        OS_RestoreInterrupts(e);
    }
}
