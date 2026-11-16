#include <nitro/types.h>

typedef struct UnkStruct_022d7754 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1ec];
    u16 unk258;
} UnkStruct_022d7754;

void Boss3PartCC_State_Unk_ov000_022d76bc(void);

void Boss3PartCC_State_WaitTimer_ov000_022d7754(UnkStruct_022d7754 *s) {
    if (s->unk258++ != 0)
        s->func = Boss3PartCC_State_Unk_ov000_022d76bc;
}
