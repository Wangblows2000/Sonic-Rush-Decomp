#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkOwner_02069b34;

typedef struct {
    u8 pad0[0x13c];
    void *res[6];
    UnkOwner_02069b34 *owner[5];
    u32 flags;
} UnkWork_02069b34;

s32 strlen(const char *s);
void *ObjData_Load(UnkOwner_02069b34 *s, const char *name, BOOL arc);
void NNS_G3dResDefaultSetup(void *p);

void StageObj_LoadMotion(void *unused, UnkWork_02069b34 *w, const char *name, UnkOwner_02069b34 *owner, BOOL flag) {
    s32 type = 0;
    s16 len;
    u16 ext;
    void *res;

    len = strlen(name);
    ext = (u16)name[len - 1] | (name[len - 2] << 8);
    switch (ext) {
    case 0x6361:
        type = 0;
        break;
    case 0x6d61:
        type = 1;
        break;
    case 0x7470:
        type = 2;
        break;
    case 0x7461:
        type = 3;
        break;
    case 0x7661:
        type = 4;
        break;
    }
    if (flag)
        w->flags |= 2 << type;
    else
        w->flags &= ~(2 << type);
    res = ObjData_Load(owner, name, flag);
    w->res[type] = res;
    if (owner == NULL)
        return;
    w->owner[type] = owner;
    if (owner->unk4 == 1)
        NNS_G3dResDefaultSetup(res);
}
