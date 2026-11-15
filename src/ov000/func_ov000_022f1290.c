#include <nitro/types.h>

typedef struct UnkStruct_022f1290 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x434 - 0x6c];
    s32 unk434;
    u16 unk438;
} UnkStruct_022f1290;

extern s32 data_ov000_02369ac0[];

void func_ov000_022f1264(void);

void func_ov000_022f1290(UnkStruct_022f1290 *s) {
    s->unk438 = 0;
    s->unk434 = 0;
    data_ov000_02369ac0[1] = 0x37;
    s->func = func_ov000_022f1264;
}
