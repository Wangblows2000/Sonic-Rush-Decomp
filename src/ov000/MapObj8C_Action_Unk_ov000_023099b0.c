#include <nitro/types.h>

typedef struct UnkStruct_023099b0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023099b0;

void MapObj8C_State_Unk_ov000_02309850(void);

void MapObj8C_Action_Unk_ov000_023099b0(UnkStruct_023099b0 *s) {
    s->func = MapObj8C_State_Unk_ov000_02309850;
}
