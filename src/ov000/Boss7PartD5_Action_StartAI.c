#include <nitro/types.h>

typedef struct UnkStruct_022eb394 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x43c - 0x6c];
    void (*func2)(void);
} UnkStruct_022eb394;

void Boss7_Unk_ov000_022ee380(UnkStruct_022eb394 *s);
void Boss7PartD5_InputState_Unk_ov000_022ea798(void);
void Boss7PartD5_State_RunAI(void);

void Boss7PartD5_Action_StartAI(UnkStruct_022eb394 *s) {
    Boss7_Unk_ov000_022ee380(s);
    s->func2 = Boss7PartD5_InputState_Unk_ov000_022ea798;
    s->func = Boss7PartD5_State_RunAI;
}
