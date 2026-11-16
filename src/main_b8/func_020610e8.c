#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u32 flags;
} UnkDef_020610e8;

typedef struct {
    UnkDef_020610e8 *def;
    void **anim;
} UnkSlot_020610e8;

typedef struct {
    u8 pad0[4];
    u32 flags;
    u8 pad8[0xc8];
    UnkSlot_020610e8 slot;
} UnkWork_020610e8;

void func_0201c070(void *a, s32 b);
void func_0201bf1c(void *a);

#pragma thumb on
void func_020610e8(UnkWork_020610e8 *w) {
    UnkSlot_020610e8 *s = &w->slot;

    if (w->slot.def != NULL && *s->anim != NULL && (w->slot.def->flags & 2)) {
        func_0201c070(s->anim, 0);
        func_0201bf1c(s->anim);
    }
    s->def = NULL;
    w->flags &= ~0x100;
}
#pragma thumb reset
