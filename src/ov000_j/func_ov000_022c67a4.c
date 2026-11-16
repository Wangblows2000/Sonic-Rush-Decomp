#include <nitro/types.h>

typedef struct {
    u8 pad0[0x228];
    s16 unk228;
} UnkStruct_022c67a4_g;

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x2cc - 0x6c];
    s16 unk2CC;
    s16 unk2CE;
} UnkStruct_022c67a4;

extern UnkStruct_022c67a4_g *gBoss1;

void Boss1Hammer_State_Unk_ov000_022c66a4(UnkStruct_022c67a4 *p);

void func_ov000_022c67a4(UnkStruct_022c67a4 *p) {
    p->unk2CC = 0;
    if (gBoss1->unk228 > 0)
        p->unk2CE = 0x1800;
    else
        p->unk2CE = -0x1800;
    p->unk68 = Boss1Hammer_State_Unk_ov000_022c66a4;
}
