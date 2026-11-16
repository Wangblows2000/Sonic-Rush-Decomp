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

void KeyframeAnim_Update(void *a);
BOOL KeyframeAnim_IsFinished(void *a);

#pragma thumb on
BOOL OptionsMenu_UpdateAnims(UnkStruct_022cc17c *p) {
    BOOL done = TRUE;
    s32 i;
    UnkItem_022cc17c *it;

    i = 0;
    it = p->items;
    for (; i < 6; i++) {
        if (it->type != 7) {
            KeyframeAnim_Update(it->anim);
            if (!KeyframeAnim_IsFinished(it->anim)) {
                done = FALSE;
            }
        }
        it++;
    }
    KeyframeAnim_Update(p->anim0);
    if (!KeyframeAnim_IsFinished(p->anim0)) {
        done = FALSE;
    }
    KeyframeAnim_Update(p->anim1);
    if (!KeyframeAnim_IsFinished(p->anim1)) {
        done = FALSE;
    }
    KeyframeAnim_Update(p->anim2);
    if (!KeyframeAnim_IsFinished(p->anim2)) {
        done = FALSE;
    }
    return done;
}
#pragma thumb reset
