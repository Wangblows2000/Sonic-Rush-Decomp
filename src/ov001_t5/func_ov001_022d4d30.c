#include <nitro/types.h>

typedef struct {
    void *res;
    u8 pad4[0x34 - 4];
    s32 active;
    u8 pad38[0x48 - 0x38];
} UnkEntry_022d4d30;

typedef struct {
    u8 pad0[4];
    UnkEntry_022d4d30 e[2];
} UnkStruct_022d4d30;

void ObjVram_Free(void *res, s32 b);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

#pragma thumb on
static inline void Free_022d4d30(UnkEntry_022d4d30 *e) {
    if (e != NULL && e->active != 0) {
        ObjVram_Free(e->res, e->active);
        {
            vu16 z = 0;
            MIi_CpuClear16(z, e, sizeof(UnkEntry_022d4d30));
        }
    }
}

void func_ov001_022d4d30(UnkStruct_022d4d30 *p) {
    s32 i;
    UnkEntry_022d4d30 *e;

    for (i = 0, e = p->e; i < 2; e++, i++) {
        Free_022d4d30(e);
    }
}
#pragma thumb reset
