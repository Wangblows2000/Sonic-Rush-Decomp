#include <nitro/types.h>

typedef struct UnkStruct_022d5d14 {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad1[0x224 - 0x210];
    u16 unk224;
    u16 unk226;
} UnkStruct_022d5d14;

void Boss3_MakeBallHittable(void);
void Boss3_MakeBallHarmful(void);

void Boss3PartCD_UpdateModeCycle(UnkStruct_022d5d14 *s) {
    if (!(s->unk20C & 8))
        return;
    if (s->unk224 > s->unk226++)
        return;
    if (s->unk20C & 0x10)
        Boss3_MakeBallHittable();
    else
        Boss3_MakeBallHarmful();
    s->unk226 = 0;
}
