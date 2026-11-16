#include <nitro/types.h>

typedef struct {
    s32 type;
    u8 anim[0x2c];
} UnkItem_022cc17c;

typedef struct {
    u8 pad0[0x208];
    u8 anim0[0x2c];
    u8 anim1[0x2c];
    u8 anim2[0x2c];
    UnkItem_022cc17c items[6];
} UnkStruct_022cc17c;

void func_02040d14(void *a);
BOOL func_02040cf4(void *a);

#pragma thumb on
BOOL func_ov001_022cc17c(UnkStruct_022cc17c *p) {
    BOOL done = TRUE;
    s32 i;
    UnkItem_022cc17c *it;

    i = 0;
    it = p->items;
    for (; i < 6; i++) {
        if (it->type != 7) {
            func_02040d14(it->anim);
            if (!func_02040cf4(it->anim)) {
                done = FALSE;
            }
        }
        it++;
    }
    func_02040d14(p->anim0);
    if (!func_02040cf4(p->anim0)) {
        done = FALSE;
    }
    func_02040d14(p->anim1);
    if (!func_02040cf4(p->anim1)) {
        done = FALSE;
    }
    func_02040d14(p->anim2);
    if (!func_02040cf4(p->anim2)) {
        done = FALSE;
    }
    return done;
}
#pragma thumb reset
