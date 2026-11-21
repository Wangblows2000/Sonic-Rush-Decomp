#include <nitro/types.h>

typedef struct UnkStruct_022ca484 UnkStruct_022ca484;
struct UnkStruct_022ca484 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022ca484 *s);
    u8 pad1[0x378 - 0x6c];
    s32 unk378;
    s32 unk37C;
    s32 unk380;
    s32 unk384;
    s32 unk388;
    s32 unk38C;
    s16 unk390;
    u16 unk392;
};

void Boss5PartD3_State_Fly(UnkStruct_022ca484 *s);

void Boss5PartD3_Action_Launch(UnkStruct_022ca484 *s, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s16 g) {
    s->unk378 = a;
    s->unk37C = b;
    s->unk380 = c;
    s->unk390 = g;
    s->unk384 = d;
    s->unk388 = e;
    s->unk38C = f;
    s->unk390 = g;
    s->unk392 = 0;
    s->func = Boss5PartD3_State_Fly;
    s->func(s);
}
