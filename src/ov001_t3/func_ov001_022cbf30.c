#include <nitro/types.h>

typedef struct {
    s32 id;
    s16 x;
    s16 y;
    u8 pad8[0x28];
} UnkItem_022cbf30;

typedef struct {
    s32 mode;
} UnkObj_022cbf30;

typedef struct UnkStruct_022cbf30 UnkStruct_022cbf30;
typedef void (*UnkFn_022cbf30)(UnkStruct_022cbf30 *p, s32 x, s32 y, s32 sel);

struct UnkStruct_022cbf30 {
    u8 pad0[4];
    s32 cur;
    u8 pad8[0x208 - 8];
    s16 ax;
    s16 ay;
    u8 pad20c[0x234 - 0x20c];
    s16 bx;
    s16 by;
    u8 pad238[0x260 - 0x238];
    s16 cx;
    s16 cy;
    u8 pad264[0x28c - 0x264];
    UnkItem_022cbf30 items[6];
    UnkObj_022cbf30 *obj;
};

extern UnkFn_022cbf30 data_ov001_0231a65c[];

void func_ov001_022cbecc(UnkObj_022cbf30 *o, s32 id, s32 x, s32 y, s32 sel);
void func_ov001_022cbcec(UnkStruct_022cbf30 *p, s32 x, s32 y);
void func_ov001_022cbe98(UnkObj_022cbf30 *o, s32 x, s32 y);
void func_ov001_022cbe50(UnkObj_022cbf30 *o, s32 x, s32 y);
void func_ov001_022cbdf0(UnkObj_022cbf30 *o, s32 x, s32 y, s32 a);

#pragma thumb on
void func_ov001_022cbf30(UnkStruct_022cbf30 *p) {
    s16 y;
    s32 i;
    UnkItem_022cbf30 *it;

    y = 0x18;
    for (i = 0, it = p->items; i < 6; it++, i++) {
        if (it->id == 7) continue;
        if (p->obj->mode == 1 || p->obj->mode == 7) {
            y = it->y;
        }
        if (i == p->cur) {
            UnkFn_022cbf30 fn = data_ov001_0231a65c[it->id];
            if (fn != NULL) fn(p, it->x, y, 1);
            func_ov001_022cbecc(p->obj, it->id, it->x, y, 1);
        } else {
            UnkFn_022cbf30 fn = data_ov001_0231a65c[it->id];
            if (fn != NULL) fn(p, it->x, y, 0);
            func_ov001_022cbecc(p->obj, it->id, it->x, y, 0);
        }
        y += 0x18;
    }
    func_ov001_022cbcec(p, p->ax, p->ay);
    func_ov001_022cbe98(p->obj, p->ax, p->ay);
    func_ov001_022cbe50(p->obj, p->bx, p->by);
    func_ov001_022cbdf0(p->obj, p->cx, p->cy, 0);
}
#pragma thumb reset
