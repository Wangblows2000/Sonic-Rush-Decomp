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

extern UnkStruct_02300ac8_t *gTaskCurrent;

BOOL ExtraBossObj_IsAlive(UnkStruct_02300ac8 *p);
void ExtraBossObj_Draw_Default(UnkStruct_02300ac8 *p);
void func_ov001_022fe92c(void *c);

void ExtraBossObj_Main(void) {
    UnkStruct_02300ac8 *p = gTaskCurrent->work;
    s32 i;

    if (p->state != NULL) {
        p->state(p);
        if (!ExtraBossObj_IsAlive(p))
            return;
    }
    if ((p->flags & 0x40000) && !(p->flags & 0x20000)) {
        if (p->draw != NULL)
            p->draw(p);
        else
            ExtraBossObj_Draw_Default(p);
    }
    if (p->flags & 0x1000)
        return;
    for (i = 0; i < 5; i++) {
        if (p->colliders[i] != NULL)
            func_ov001_022fe92c(p->colliders[i]);
    }
}
