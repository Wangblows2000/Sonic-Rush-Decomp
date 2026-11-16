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

u32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(u32 e);
void MI_CpuCopy8(const void *src, void *dst, u32 size);

s32 func_0203af98(u32 n) {
    u32 e;
    u16 bit;
    UnkStruct_022b89e8 tmp;

    e = OS_DisableInterrupts();
    bit = 1 << n;
    if (!(data_022b89e8.unk2 & bit)) {
        OS_RestoreInterrupts(e);
        return 0;
    }
    MI_CpuCopy8(&data_022b89e8, &tmp, sizeof(tmp));
    OS_RestoreInterrupts(e);
    if (tmp.unk4 & bit)
        return 2;
    if (tmp.unk6 & bit)
        return 3;
    if (tmp.unk8 & bit)
        return 4;
    if (tmp.unka & bit)
        return 5;
    if (tmp.unkc & bit)
        return 6;
    return 1;
}
