#include <nitro/types.h>

typedef struct UnkStruct_022eb364 UnkStruct_022eb364;
struct UnkStruct_022eb364 {
    u8 pad[0x3d8];
    u8 unk3D8[0x64];
    s32 (*func)(UnkStruct_022eb364 *s);
    void (*func2)(UnkStruct_022eb364 *s);
};

void Pad_Update(void *p, s32 a);

void Boss7PartD5_State_RunAI(UnkStruct_022eb364 *s) {
    Pad_Update(s->unk3D8, s->func(s));
    s->func2(s);
}
