#include <nitro/types.h>

typedef struct UnkStruct_02305904 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02305904;

void CranePartED_State_Unk_ov000_023056fc(void);

void CranePartED_Action_Init(UnkStruct_02305904 *s) {
    s->func = CranePartED_State_Unk_ov000_023056fc;
}
