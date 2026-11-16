#include <nitro/types.h>

typedef struct UnkStruct_02310174_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02310174_v;

typedef struct UnkStruct_02310174_p {
    s32 x;
    s32 y;
} UnkStruct_02310174_p;

typedef struct UnkStruct_02310174_o {
    u8 pad0[0x20];
    UnkStruct_02310174_v pos;
    u8 pad2C[0x60 - 0x2c];
    u16 unk60;
    u8 pad62[0xc4 - 0x62];
    void *unkC4;
    u8 padC8[0xdc - 0xc8];
    struct UnkStruct_02310174_o *next;
} UnkStruct_02310174_o;

typedef struct UnkStruct_02310174 {
    u8 pad0[0xec];
    UnkStruct_02310174_o *head;
    u8 padF0[4];
    UnkStruct_02310174_o sentinel;
} UnkStruct_02310174;

BOOL func_ov001_02310248(void *a, UnkStruct_02310174_o *o);
BOOL func_020254d4(UnkStruct_02310174_v *v, s32 *x, s32 *y);
BOOL func_ov001_0230ae34(void *a, UnkStruct_02310174_p *p, UnkStruct_02310174_p *q);

UnkStruct_02310174_o *func_ov001_02310174(void *a, UnkStruct_02310174 *list, s32 x, s32 y) {
    UnkStruct_02310174_o *o;
    UnkStruct_02310174_p p;
    UnkStruct_02310174_p q;
    s32 sx;
    s32 sy;

    for (o = list->head; o != &list->sentinel; o = o->next) {
        if ((o->unk60 & 9) == 0 && o->unkC4 != NULL && (o->unk60 & 0x100) == 0 && func_ov001_02310248(a, o)) {
            func_020254d4(&o->pos, &sx, &sy);
            p.x = sx << 12;
            p.y = sy << 12;
            q.x = x << 12;
            q.y = y << 12;
            if (func_ov001_0230ae34(o->unkC4, &p, &q))
                return o;
        }
    }
    return NULL;
}
