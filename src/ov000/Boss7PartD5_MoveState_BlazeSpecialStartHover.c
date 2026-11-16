#include <nitro/types.h>

typedef struct UnkStruct_022e7a00 {
    u8 pad[0x440];
    void (*func)(void);
    u8 pad1[0x26];
    u16 unk46A;
} UnkStruct_022e7a00;

void Boss7PartD5_MoveState_BlazeSpecialHoverDelay(void);

void Boss7PartD5_MoveState_BlazeSpecialStartHover(UnkStruct_022e7a00 *s) {
    s->unk46A = 0;
    s->func = Boss7PartD5_MoveState_BlazeSpecialHoverDelay;
}
