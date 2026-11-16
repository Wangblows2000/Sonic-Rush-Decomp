#include <nitro/types.h>

typedef struct UnkStruct_02304270 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02304270;

void MapObj74_State_Unk_ov000_0230422c(void);

void MapObj74_Action_Unk_ov000_02304270(UnkStruct_02304270 *s) {
    s->func = MapObj74_State_Unk_ov000_0230422c;
}
