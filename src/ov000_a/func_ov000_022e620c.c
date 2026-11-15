#include <nitro/types.h>

typedef struct UnkStruct_022e620c {
    u8 pad[0xd5e];
    u16 unkD5E;
    u8 pad2[0x10ec - 0xd60];
    u16 state;
} UnkStruct_022e620c;

extern UnkStruct_022e620c *data_ov000_02369b4c;
extern u32 data_ov000_023658bc[];
extern u8 data_ov000_023638c4[];

BOOL func_ov000_022e5d84(u8 *p, u32 v, s32 k);
void G3_MultMtx33(void *m);

void func_ov000_022e620c(u8 *p) {
    switch (data_ov000_02369b4c->state) {
    case 1:
        if (!func_ov000_022e5d84(p, 0x1d, data_ov000_023658bc[1]))
            return;
        data_ov000_02369b4c->state++;
        return;
    case 2:
        if (!func_ov000_022e5d84(p, 0x1e, data_ov000_023658bc[0]))
            return;
        data_ov000_02369b4c->state = 0;
        *(u32 *)(p + 0x24) = 0;
        p[0x92] = 0;
        return;
    case 0:
        data_ov000_02369b4c->state++;
        if (data_ov000_02369b4c->unkD5E == 0)
            return;
        G3_MultMtx33(data_ov000_023638c4);
        break;
    }
}
