#include <nitro/types.h>

typedef struct UnkStruct_0231a0a0 {
    u8 pad[0x34];
    u16 unk34;
    u8 pad2[0x68 - 0x36];
    void (*func)(void);
} UnkStruct_0231a0a0;

void GmkTable_State_Main(void);

void GmkTable_Action_Init(UnkStruct_0231a0a0 *s) {
    s->func = GmkTable_State_Main;
    s->unk34 = 0x100;
}
