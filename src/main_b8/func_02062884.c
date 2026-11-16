#include <nitro/types.h>

typedef struct {
    u32 pad0;
    u32 pad4;
    s32 unk8;
} UnkSlot_02062884;

typedef struct {
    void (*state)(void *);
    u8 pad4[0xcc];
    UnkSlot_02062884 slot;
} UnkWork_02062884;

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkRes_02062884;

void func_02061288(UnkWork_02062884 *w);
u32 func_02060e70(s32 a);
UnkRes_02062884 **func_02060384(void);
void func_02036754(void *a, s32 b, s32 c);
void func_02062860(void *w);

#pragma thumb on
void func_02062884(UnkWork_02062884 *w) {
    func_02061288(w);
    if (func_02060e70(3) & 0x50) {
        UnkSlot_02062884 *s = &w->slot;
        UnkRes_02062884 *r = *func_02060384();
        if (r != NULL)
            func_02036754(NULL, r->unk4, s->unk8);
        w->state = func_02062860;
    }
}
#pragma thumb reset
