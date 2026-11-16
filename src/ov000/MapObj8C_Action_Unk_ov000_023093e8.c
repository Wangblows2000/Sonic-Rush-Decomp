#include <nitro/types.h>

typedef struct UnkStruct_023093e8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023093e8;

void MapObj8C_State_Unk_ov000_023093e4(void);

void MapObj8C_Action_Unk_ov000_023093e8(UnkStruct_023093e8 *s) {
    s->func = MapObj8C_State_Unk_ov000_023093e4;
}
