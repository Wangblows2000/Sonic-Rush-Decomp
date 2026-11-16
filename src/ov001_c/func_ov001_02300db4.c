#include <nitro/types.h>

typedef struct UnkStruct_02300db4_v {
    s32 x, y, z;
} UnkStruct_02300db4_v;

typedef struct UnkStruct_02300db4_o {
    u8 pad0[0x14];
    u32 flags;
    UnkStruct_02300db4_v pos;
    UnkStruct_02300db4_v prev;
    u8 pad30[0x3c - 0x30];
    u16 ang[3];
    u8 pad42[0x50 - 0x42];
    s32 vel[3];
    s32 acc[3];
    u16 angVel[3];
    u16 angAcc[3];
} UnkStruct_02300db4_o;

typedef struct UnkStruct_02300db4 {
    u8 pad0[0xbc];
    UnkStruct_02300db4_o *objs[0x2e];
} UnkStruct_02300db4;

extern UnkStruct_02300db4 *data_ov001_0231f530;

BOOL func_ov001_02300be4(UnkStruct_02300db4_o *o);

void func_ov001_02300db4(void) {
    s32 i;
    UnkStruct_02300db4_o *o;
    s32 v;
    s32 s;

    for (i = 0; i < 0x2e; i++) {
        o = data_ov001_0231f530->objs[i];
        if (!func_ov001_02300be4(o))
            continue;
        if (o->flags & 0x80000)
            continue;
        o->prev = o->pos;
        if (!(o->flags & 0x200000)) {
            if (o->flags & 0x100) {
                v = o->vel[0];
                s = v + o->acc[0];
                if ((s >= 0 && v < 0) || (s <= 0 && v > 0) || s == 0) {
                    o->vel[0] = 0;
                    o->acc[0] = 0;
                    o->flags &= ~0x100;
                }
            }
            if (o->flags & 0x200) {
                v = o->vel[1];
                s = v + o->acc[1];
                if ((s >= 0 && v < 0) || (s <= 0 && v > 0) || s == 0) {
                    o->vel[1] = 0;
                    o->acc[1] = 0;
                    o->flags &= ~0x200;
                }
            }
            if (o->flags & 0x400) {
                v = o->vel[2];
                s = v + o->acc[2];
                if ((s >= 0 && v < 0) || (s <= 0 && v > 0) || s == 0) {
                    o->vel[2] = 0;
                    o->acc[2] = 0;
                    o->flags &= ~0x400;
                }
            }
            o->vel[0] += o->acc[0];
            o->vel[1] += o->acc[1];
            o->vel[2] += o->acc[2];
            o->pos.x += o->vel[0];
            o->pos.y += o->vel[1];
            o->pos.z += o->vel[2];
        }
        if (!(o->flags & 0x400000)) {
            o->angVel[0] += o->angAcc[0];
            o->angVel[1] += o->angAcc[1];
            o->angVel[2] += o->angAcc[2];
            o->ang[0] += o->angVel[0];
            o->ang[1] += o->angVel[1];
            o->ang[2] += o->angVel[2];
        }
    }
}
