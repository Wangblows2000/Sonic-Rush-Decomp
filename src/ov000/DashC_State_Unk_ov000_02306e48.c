#include <nitro/types.h>

typedef struct UnkStruct_02306e48 {
    u8 pad[0x13e];
    u16 unk13E;
} UnkStruct_02306e48;

void StageObj_Draw2D(UnkStruct_02306e48 *s, void *a, void *b);

void DashC_State_Unk_ov000_02306e48(UnkStruct_02306e48 *s) {
    StageObj_Draw2D(s, (u8 *)s + 0x20c, (u8 *)s + 0x254);
    s->unk13E &= ~0x100;
}
