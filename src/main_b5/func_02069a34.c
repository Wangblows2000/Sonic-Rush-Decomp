#include <nitro/types.h>

typedef struct UnkObj_02069a34 {
    u8 pad0[0xa0];
    void *res;
    struct UnkOwner_02069a34 *owner;
    u32 flags;
    struct UnkObj_02069a34 *next;
} UnkObj_02069a34;

typedef struct UnkOwner_02069a34 {
    u8 pad0[4];
    u16 unk4;
} UnkOwner_02069a34;

typedef struct {
    u8 pad0[0x128];
    UnkObj_02069a34 *list;
} UnkWork_02069a34;

void *func_020687b8(UnkOwner_02069a34 *s, const char *name, void *arc);
void func_020319f0(UnkObj_02069a34 *o, s32 a);
void func_020319a4(UnkObj_02069a34 *o, void *res, s32 a, u16 b, u16 c);
void func_020257f8(void *p);

void func_02069a34(UnkWork_02069a34 *w, UnkObj_02069a34 *o, const char *name, s32 a, u16 b, u16 c, UnkOwner_02069a34 *owner, void *arc) {
    UnkObj_02069a34 *p;
    void *res;

    p = w->list;
    if (p != NULL) {
        for (;;) {
            if (p->next == NULL) {
                p->next = o;
                break;
            }
            p = p->next;
        }
    } else {
        w->list = o;
    }
    func_020319f0(o, 0);
    if (arc != NULL)
        o->flags |= 1;
    res = func_020687b8(owner, name, arc);
    if (res == NULL && arc != NULL) {
        o->flags &= ~1;
        res = func_020687b8(owner, name, NULL);
    }
    o->res = res;
    if (owner != NULL) {
        o->owner = owner;
        if (owner->unk4 == 1)
            func_020257f8(res);
    }
    if (res == NULL)
        return;
    func_020319a4(o, res, a, c, b);
}
