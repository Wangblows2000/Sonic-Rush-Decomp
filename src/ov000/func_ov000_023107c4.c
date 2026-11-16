#include <nitro/types.h>

typedef struct UnkStruct_023107c4 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_023107c4;

void StageObj_SetAnimation(UnkStruct_023107c4 *s, s32 a);
void Marker_State_Unk_ov000_02310730(void);

void func_ov000_023107c4(UnkStruct_023107c4 *s) {
    StageObj_SetAnimation(s, 0);
    s->unk5C |= 4;
    s->func = Marker_State_Unk_ov000_02310730;
}
