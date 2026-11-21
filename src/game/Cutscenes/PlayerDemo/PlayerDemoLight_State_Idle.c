#include <nitro/types.h>

typedef struct {
    u32 pad0;
    u32 flags;
} UnkSub_02056084;

typedef struct {
    u32 pad0;
    UnkSub_02056084 *sub;
} UnkObj_02056084;

void PlayerDemoLight_Action_Flash(UnkObj_02056084 *o, void *p);
void PlayerDemoLight_ApplyAlt(UnkObj_02056084 *o);

#pragma thumb on
void PlayerDemoLight_State_Idle(UnkObj_02056084 *o, void *p) {
    UnkSub_02056084 *s = o->sub;

    if (s->flags & 0x1000) {
        s->flags &= ~0x1000;
        PlayerDemoLight_Action_Flash(o, p);
    }
    if (o->sub->flags & 0x8000) {
        o->sub->flags &= ~0x8000;
        PlayerDemoLight_ApplyAlt(o);
    }
}
#pragma thumb reset
