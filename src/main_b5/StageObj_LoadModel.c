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

void *ObjData_Load(UnkOwner_02069c74 *s, const char *name, void *arc);
void Model_Init(UnkObj_02069c74 *o, s32 a);
void Model_SetResource(UnkObj_02069c74 *o, void *res, s32 a);
void func_020257f8(void *p);

void StageObj_LoadModel(UnkWork_02069c74 *w, UnkObj_02069c74 *o, const char *name, s32 a, UnkOwner_02069c74 *owner, void *arc) {
    void *res;

    w->obj = o;
    Model_Init(o, 0);
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
            func_020257f8(res);
    }
    if (res == NULL)
        return;
    Model_SetResource(o, res, a);
}
