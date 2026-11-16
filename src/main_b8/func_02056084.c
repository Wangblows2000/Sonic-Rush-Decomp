#include <nitro/types.h>

typedef struct {
    u32 pad0;
    u32 flags;
} UnkSub_02056084;

typedef struct {
    u32 pad0;
    UnkSub_02056084 *sub;
} UnkObj_02056084;

void func_02056038(UnkObj_02056084 *o, void *p);
void func_02055e48(UnkObj_02056084 *o);

#pragma thumb on
void func_02056084(UnkObj_02056084 *o, void *p) {
    UnkSub_02056084 *s = o->sub;

    if (s->flags & 0x1000) {
        s->flags &= ~0x1000;
        func_02056038(o, p);
    }
    if (o->sub->flags & 0x8000) {
        o->sub->flags &= ~0x8000;
        func_02055e48(o);
    }
}
#pragma thumb reset
