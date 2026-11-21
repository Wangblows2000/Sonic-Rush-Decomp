#include <nitro/types.h>

typedef struct UnkStruct_022dc82c {
    u8 pad[0x14];
    s32 unk14;
    u8 pad1[0x240];
    s32 unk258;
    u8 pad2[0x22c];
    void (*func)(void);
    s32 unk48C;
} UnkStruct_022dc82c;

void Boss4PartD0_SubState_Launch(void);

void Boss4PartD0_StartLaunch(UnkStruct_022dc82c *s) {
    s->func = Boss4PartD0_SubState_Launch;
    s->unk48C = 0;
    s->unk14 = 0x20000;
    s->unk258 = -0x200000;
}
