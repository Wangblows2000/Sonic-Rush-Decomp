#include <nitro/types.h>

typedef struct UnkStruct_022eb124_2 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022eb124_2;

typedef struct UnkStruct_022eb124 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x43c - 0x14];
    void (*func)(void);
    u8 pad2[0x456 - 0x440];
    s16 unk456;
} UnkStruct_022eb124;

extern UnkStruct_022eb124_2 *gPlayers;

void func_ov000_022ea614(void);

void func_ov000_022eb124(UnkStruct_022eb124 *s) {
    s->unk456 = (gPlayers->unk10 - s->unk10) >> 9;
    s->func = func_ov000_022ea614;
}
