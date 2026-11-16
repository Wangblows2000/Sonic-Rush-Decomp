#include <nitro/types.h>

typedef struct UnkStruct_022edc94 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1c4];
    u16 unk230;
} UnkStruct_022edc94;

void Boss7_State_Unk_ov000_022edc70(void);

void Boss7_State_Unk_ov000_022edc94(UnkStruct_022edc94 *s) {
    s->unk230 = 0;
    s->func = Boss7_State_Unk_ov000_022edc70;
}
