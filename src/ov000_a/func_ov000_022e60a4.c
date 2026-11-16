#include <nitro/types.h>

typedef struct UnkStruct_022e60a4 {
    u8 pad[0xd5e];
    u16 unkD5E;
    u8 pad2[0x10ec - 0xd60];
    u16 state;
} UnkStruct_022e60a4;

extern UnkStruct_022e60a4 *gBoss6;
extern u32 data_ov000_023658bc[];
extern u8 data_ov000_023638c4[];

BOOL Boss6_StoreNodeMatrix(u8 *p, u32 v, s32 k);
void G3_MultMtx33(void *m);

void func_ov000_022e60a4(u8 *p) {
    switch (gBoss6->state) {
    case 1:
        if (!Boss6_StoreNodeMatrix(p, 0x1c, data_ov000_023658bc[2]))
            return;
        gBoss6->state++;
        return;
    case 2:
        if (!Boss6_StoreNodeMatrix(p, 0x1d, data_ov000_023658bc[1]))
            return;
        gBoss6->state++;
        return;
    case 3:
        if (!Boss6_StoreNodeMatrix(p, 0x1b, data_ov000_023658bc[3]))
            return;
        gBoss6->state++;
        return;
    case 4:
        if (!Boss6_StoreNodeMatrix(p, 0x1e, data_ov000_023658bc[0]))
            return;
        gBoss6->state = 0;
        *(u32 *)(p + 0x24) = 0;
        p[0x92] = 0;
        return;
    case 0:
        gBoss6->state++;
        if (gBoss6->unkD5E == 0)
            return;
        G3_MultMtx33(data_ov000_023638c4);
        break;
    }
}
