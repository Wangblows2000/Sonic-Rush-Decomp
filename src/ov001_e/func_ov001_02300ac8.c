#include <nitro/types.h>

typedef struct UnkStruct_02300ac8 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x74 - 0x18];
    void (*state)(struct UnkStruct_02300ac8 *p);
    void (*draw)(struct UnkStruct_02300ac8 *p);
    u8 pad7C[0x9c - 0x7c];
    void *colliders[5];
} UnkStruct_02300ac8;

typedef struct UnkStruct_02300ac8_t {
    u8 pad0[0x10];
    UnkStruct_02300ac8 *work;
} UnkStruct_02300ac8_t;

extern UnkStruct_02300ac8_t *data_022b4574;

BOOL func_ov001_02300be4(UnkStruct_02300ac8 *p);
void func_ov001_02300a64(UnkStruct_02300ac8 *p);
void func_ov001_022fe92c(void *c);

void func_ov001_02300ac8(void) {
    UnkStruct_02300ac8 *p = data_022b4574->work;
    s32 i;

    if (p->state != NULL) {
        p->state(p);
        if (!func_ov001_02300be4(p))
            return;
    }
    if ((p->flags & 0x40000) && !(p->flags & 0x20000)) {
        if (p->draw != NULL)
            p->draw(p);
        else
            func_ov001_02300a64(p);
    }
    if (p->flags & 0x1000)
        return;
    for (i = 0; i < 5; i++) {
        if (p->colliders[i] != NULL)
            func_ov001_022fe92c(p->colliders[i]);
    }
}
