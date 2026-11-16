#include <nitro/types.h>

typedef struct UnkStruct_022edc70 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1c4];
    u16 unk230;
} UnkStruct_022edc70;

void Boss7_Action_StartTalkDemo(void);

void Boss7_State_Unk_ov000_022edc70(UnkStruct_022edc70 *s) {
    if (s->unk230++ > 0x3c)
        s->func = Boss7_Action_StartTalkDemo;
}
