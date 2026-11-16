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

s32 func_0202c5f0(const char *s);
void *func_020687b8(UnkOwner_02069b34 *s, const char *name, BOOL arc);
void func_020257f8(void *p);

void func_02069b34(void *unused, UnkWork_02069b34 *w, const char *name, UnkOwner_02069b34 *owner, BOOL flag) {
    s32 type = 0;
    s16 len;
    u16 ext;
    void *res;

    len = func_0202c5f0(name);
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
    res = func_020687b8(owner, name, flag);
    w->res[type] = res;
    if (owner == NULL)
        return;
    w->owner[type] = owner;
    if (owner->unk4 == 1)
        func_020257f8(res);
}
