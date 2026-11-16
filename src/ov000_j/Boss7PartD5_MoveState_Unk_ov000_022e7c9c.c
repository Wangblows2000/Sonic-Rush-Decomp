#include <nitro/types.h>

typedef struct {
    u8 pad0[0x440];
    void *unk440;
    u8 pad444[0x468 - 0x444];
    s16 unk468;
    u8 pad46A[2];
    s32 unk46C;
} UnkStruct_022e7c9c;

typedef struct {
    u8 pad0[0x22c];
    s16 unk22C;
} UnkStruct_022e7c9c_g;

extern UnkStruct_022e7c9c_g *gBoss7;

void Boss7PartD5_Unk_ov000_022e8538(UnkStruct_022e7c9c *p);
void Boss7PartD5_MoveState_Unk_ov000_022e7c24(UnkStruct_022e7c9c *p);

void Boss7PartD5_MoveState_Unk_ov000_022e7c9c(UnkStruct_022e7c9c *p) {
    gBoss7->unk22C = -8;
    p->unk468 -= 0x20;
    if (p->unk468 < 0)
        p->unk468 = 0;
    p->unk46C -= 0x666;
    Boss7PartD5_Unk_ov000_022e8538(p);
    if (p->unk46C < 0)
        p->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e7c24;
}
