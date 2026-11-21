#include <nitro/types.h>

typedef struct UnkStruct_022fe92c_o {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    s32 z;
    u8 pad24[0x40 - 0x24];
    u16 rot;
} UnkStruct_022fe92c_o;

typedef struct UnkStruct_022fe92c {
    UnkStruct_022fe92c_o *owner;
    u8 pad4[4];
    u32 flags;
    u8 padC[0x28 - 0xc];
    s32 x;
    s32 y;
    s32 z;
    u16 rot;
} UnkStruct_022fe92c;

extern UnkStruct_022fe92c *data_ov001_0231f430[3][20];
extern s32 data_ov001_0231f334[3];
extern UnkStruct_022fe92c *data_ov001_0231f340[3][20];
extern s32 data_ov001_0231f328[3];

void ExtraBossObj_RegisterColliders(UnkStruct_022fe92c *p) {
    UnkStruct_022fe92c_o *o = p->owner;
    s32 i;
    u32 bit;

    if (p->flags & 0x100) {
        for (i = 0, bit = 1; i < 3; i++, bit <<= 1) {
            if (p->flags & bit) {
                if (!(p->flags & 0x1000)) {
                    p->x = o->x;
                    p->y = o->y;
                    p->z = o->z;
                }
                if (!(p->flags & 0x2000))
                    p->rot = 0x10000 - o->rot;
                if (data_ov001_0231f334[i] < 20) {
                    data_ov001_0231f430[i][data_ov001_0231f334[i]] = p;
                    data_ov001_0231f334[i]++;
                }
            }
        }
    } else {
        for (i = 0, bit = 1; i < 3; i++, bit <<= 1) {
            if (p->flags & bit) {
                if (!(p->flags & 0x1000)) {
                    p->x = o->x;
                    p->y = o->y;
                    p->z = o->z;
                }
                if (!(p->flags & 0x2000))
                    p->rot = 0x10000 - o->rot;
                if (data_ov001_0231f328[i] < 20) {
                    data_ov001_0231f340[i][data_ov001_0231f328[i]] = p;
                    data_ov001_0231f328[i]++;
                }
            }
        }
    }
}
