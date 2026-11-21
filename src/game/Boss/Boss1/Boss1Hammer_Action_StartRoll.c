#include <nitro/types.h>

typedef struct UnkStruct_022c6924 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x2bc - 0x6c];
    u16 unk2bc;
} UnkStruct_022c6924;

void Boss1Hammer_State_WaitRelease(void);

void Boss1Hammer_Action_StartRoll(UnkStruct_022c6924 *s) {
    s->unk2bc = 0;
    s->func = Boss1Hammer_State_WaitRelease;
}
