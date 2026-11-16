#include <nitro/types.h>

typedef struct UnkStruct_022edbb8_2 {
    u8 pad[0x43c];
    void (*func)(void);
} UnkStruct_022edbb8_2;

typedef struct UnkStruct_022edbb8 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x214 - 0x6c];
    UnkStruct_022edbb8_2 *unk214;
    u8 pad2[0x230 - 0x218];
    u16 unk230;
} UnkStruct_022edbb8;

void Boss7PartD5_InputState_Unk_ov000_022e9db8(void);
void Boss7_State_Unk_ov000_022ed8b4(void);

void Boss7_State_Unk_ov000_022edbb8(UnkStruct_022edbb8 *s) {
    if (s->unk230++ > 0x3c) {
        s->unk214->func = Boss7PartD5_InputState_Unk_ov000_022e9db8;
        s->func = Boss7_State_Unk_ov000_022ed8b4;
    }
}
