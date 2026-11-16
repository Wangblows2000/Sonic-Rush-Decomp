#include <nitro/types.h>

typedef struct UnkStruct_023136bc {
    u8 pad[0x2e];
    s16 unk2E;
} UnkStruct_023136bc;

s32 ObjSpd_DownSet(s32 a, s32 b);

void RockBreakPart_State_Unk_ov000_023136bc(UnkStruct_023136bc *s) {
    s->unk2E = ObjSpd_DownSet(s->unk2E, 10);
}
