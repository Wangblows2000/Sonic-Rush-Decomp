#include <nitro/types.h>

typedef struct UnkStruct_023090d8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023090d8;

void MapObj8D_State_Unk_ov000_02309034(void);

void MapObj8D_Action_Unk_ov000_023090d8(UnkStruct_023090d8 *s) {
    s->func = MapObj8D_State_Unk_ov000_02309034;
}
