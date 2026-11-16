#include <nitro/types.h>

typedef struct UnkStruct_02308800 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02308800;

void MapObj8F_State_Unk_ov000_0230875c(void);

void MapObj8F_Action_Unk_ov000_02308800(UnkStruct_02308800 *s) {
    s->func = MapObj8F_State_Unk_ov000_0230875c;
}
