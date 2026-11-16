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

void *ObjData_Load(UnkOwner_02069a34 *s, const char *name, void *arc);
void Model_InitShape(UnkObj_02069a34 *o, s32 a);
void Model_SetShapeResource(UnkObj_02069a34 *o, void *res, s32 a, u16 b, u16 c);
void NNS_G3dResDefaultSetup(void *p);

void StageObj_LoadModelSimple(UnkWork_02069a34 *w, UnkObj_02069a34 *o, const char *name, s32 a, u16 b, u16 c, UnkOwner_02069a34 *owner, void *arc) {
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
    Model_InitShape(o, 0);
    if (arc != NULL)
        o->flags |= 1;
    res = ObjData_Load(owner, name, arc);
    if (res == NULL && arc != NULL) {
        o->flags &= ~1;
        res = ObjData_Load(owner, name, NULL);
    }
    o->res = res;
    if (owner != NULL) {
        o->owner = owner;
        if (owner->unk4 == 1)
            NNS_G3dResDefaultSetup(res);
    }
    if (res == NULL)
        return;
    Model_SetShapeResource(o, res, a, c, b);
}
