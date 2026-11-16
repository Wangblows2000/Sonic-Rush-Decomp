#include <nitro/types.h>

typedef struct UnkEnt_022ed924 {
    void *unk0;
    u8 pad4[0x30];
    s32 unk34;
    u8 pad38[0x48 - 0x38];
} UnkEnt_022ed924;

typedef struct UnkStruct_022ed924 {
    u8 pad0[0x64];
    UnkEnt_022ed924 ents[21];
} UnkStruct_022ed924;

void func_02039bb0(void *p, s32 a);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

#pragma thumb on
void func_ov001_022ed924(UnkStruct_022ed924 *p) {
    s32 i;
    UnkEnt_022ed924 *e;
    for (i = 0; i < 21; i++) {
        e = &p->ents[i];
        vu16 zero;
        if (e != NULL && e->unk34 != 0)
            func_02039bb0(e->unk0, e->unk34);
        zero = 0;
        MIi_CpuClear16(zero, e, sizeof(UnkEnt_022ed924));
    }
}
#pragma thumb reset
