#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkOwner_02069c74;

typedef struct {
    u8 pad0[0x138];
    void *res;
    u8 pad13c[0x14];
    UnkOwner_02069c74 *owner;
    u8 pad154[0x14];
    u32 flags;
} UnkObj_02069c74;

typedef struct {
    u8 pad0[0x124];
    UnkObj_02069c74 *obj;
} UnkWork_02069c74;

void *func_020687b8(UnkOwner_02069c74 *s, const char *name, void *arc);
void func_02031c1c(UnkObj_02069c74 *o, s32 a);
void func_02031bd0(UnkObj_02069c74 *o, void *res, s32 a);
void func_020257f8(void *p);

void func_02069c74(UnkWork_02069c74 *w, UnkObj_02069c74 *o, const char *name, s32 a, UnkOwner_02069c74 *owner, void *arc) {
    void *res;

    w->obj = o;
    func_02031c1c(o, 0);
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
    func_02031bd0(o, res, a);
}
