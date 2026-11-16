#include <nitro/types.h>

typedef struct UnkStruct_022edcb0_2 {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022edcb0_2;

typedef struct UnkStruct_022edcb0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x210 - 0x6c];
    s32 unk210;
} UnkStruct_022edcb0;

extern UnkStruct_022edcb0_2 data_ov000_0236a444;

void Boss7_State_Unk_ov000_022edc94(void);

void Boss7_State_Unk_ov000_022edcb0(UnkStruct_022edcb0 *s) {
    s->unk210 = 3;
    data_ov000_0236a444.unkDC = 2;
    s->func = Boss7_State_Unk_ov000_022edc94;
}
