#include <nitro/types.h>

typedef struct UnkStruct_022c7274_3 {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022c7274_3;

typedef struct UnkStruct_022c7274_4 {
    u8 pad[0x20c];
    UnkStruct_022c7274_3 unk20C[4];
} UnkStruct_022c7274_4;

typedef struct UnkStruct_022c7274_2 {
    u8 pad[0x214];
    UnkStruct_022c7274_4 *unk214;
} UnkStruct_022c7274_2;

typedef struct UnkStruct_022c7274_5 {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_022c7274_5;

extern UnkStruct_022c7274_2 *gBoss1;
extern UnkStruct_022c7274_5 *gPlayers;

BOOL func_ov000_022cc1f0(void *a, UnkStruct_022c7274_4 *o);

void func_ov000_022c7274(u32 mask) {
    UnkStruct_022c7274_4 *o = gBoss1->unk214;
    BOOL r = func_ov000_022cc1f0(gPlayers->unk18, o);
    u32 i;
    UnkStruct_022c7274_3 *p;
    for (i = 0, p = o->unk20C; i < 4; i++, p++) {
        if (mask & (1 << i)) {
            if (r)
                p->unkA |= 4;
            else
                p->unkA &= ~4;
        }
    }
}
