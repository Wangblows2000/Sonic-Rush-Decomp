#include <nitro/types.h>

typedef struct UnkStruct_02326f20_2 {
    u8 unk0;
    u8 pad[0x53];
    u32 unk54;
    u8 pad1[0x10];
    void (*func)(void);
} UnkStruct_02326f20_2;

typedef struct UnkStruct_02326f20 {
    u8 pad[0x10];
    UnkStruct_02326f20_2 *unk10;
} UnkStruct_02326f20;

void EffTrail_State_Follow(void);

void EffTrail_StartFollow(UnkStruct_02326f20 *s) {
    UnkStruct_02326f20_2 *p = s->unk10;
    if (p->unk0 == 1)
        return;
    if (!(p->unk54 & 4))
        p->func = EffTrail_State_Follow;
}
