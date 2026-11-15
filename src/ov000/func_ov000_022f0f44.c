#include <nitro/types.h>

typedef struct UnkStruct_022f0f44_2 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022f0f44_2;

typedef struct UnkStruct_022f0f44 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(void);
    u8 pad2[0x44e - 0x6c];
    u16 unk44E;
} UnkStruct_022f0f44;

typedef struct UnkStruct_022f0f44_3 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u32 unk18;
} UnkStruct_022f0f44_3;

extern UnkStruct_022f0f44_2 *data_ov000_02369b84;
extern UnkStruct_022f0f44_3 data_022c4560;
extern u16 data_ov000_02363dd8[];
extern s32 data_ov000_02369ac0[];

void func_ov000_022f0f28(void);

void func_ov000_022f0f44(UnkStruct_022f0f44 *s) {
    UnkStruct_022f0f44_2 *p = data_ov000_02369b84;
    BOOL b;
    s32 i;
    s->unk2E = 0;
    b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        i = 1;
    else
        i = data_022c4560.unk14;
    s->unk44E = data_ov000_02363dd8[i];
    data_022c4560.unk18 &= ~1;
    p->unk20C |= 0x10;
    data_ov000_02369ac0[1] = 0x38;
    s->func = func_ov000_022f0f28;
}
