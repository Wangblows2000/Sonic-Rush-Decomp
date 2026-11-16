#include <nitro/types.h>

typedef struct UnkStruct_022ec294 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x4c];
    u16 unk5C;
    u8 pad2[0xa];
    void (*func)(void);
} UnkStruct_022ec294;

void Boss7PartD6_State_Vanish(void);

void Boss7PartD6_State_Active(UnkStruct_022ec294 *s) {
    if (s->unkC > 0)
        s->unkC--;
    if (!(s->unk5C & 8))
        return;
    if (s->unkC == 0)
        s->func = Boss7PartD6_State_Vanish;
}
