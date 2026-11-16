#include <nitro/types.h>

typedef struct UnkStruct_022d8ba8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d8ba8;

void Boss3PartCC_State_Unk_ov000_022d8a90(void);

void Boss3PartCC_Action_Unk_ov000_022d8ba8(UnkStruct_022d8ba8 *s) {
    s->func = Boss3PartCC_State_Unk_ov000_022d8a90;
}
