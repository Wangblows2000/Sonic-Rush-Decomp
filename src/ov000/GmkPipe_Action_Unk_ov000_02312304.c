#include <nitro/types.h>

typedef struct UnkStruct_02312304 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02312304;

void GmkPipe_State_Unk_ov000_023122d8(void);

void GmkPipe_Action_Unk_ov000_02312304(UnkStruct_02312304 *s) {
    s->func = GmkPipe_State_Unk_ov000_023122d8;
}
