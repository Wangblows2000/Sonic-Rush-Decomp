#include <nitro/types.h>

typedef struct UnkNode_0206cc90 {
    u16 used : 8;
    u16 prio : 2;
    u16 rest : 6;
    u8 pad2[2];
    void *data;
    struct UnkNode_0206cc90 *next;
} UnkNode_0206cc90;

extern void *data_022c4f18;

void MIi_CpuClear16(u16 data, void *dest, u32 size);

UnkNode_0206cc90 *func_0206cc90(UnkNode_0206cc90 *n, u8 a, s32 prio, s32 size) {
    if (size == 0) {
        vu16 z = 0;
        MIi_CpuClear16(z, n, size);
    } else {
        vu16 z = 0;
        MIi_CpuClear16(z, n, 4);
    }
    n->used = a;
    n->prio = (u8)prio;
    if (data_022c4f18 == NULL) {
        if (size == 0)
            size = 4;
        n->rest = (u8)size;
    }
    return n;
}
