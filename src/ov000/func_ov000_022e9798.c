#include <nitro/types.h>

typedef struct UnkStruct_022e9798 {
    u8 pad[0x440];
    void (*func)(void);
    u8 pad1[0x464 - 0x444];
    u16 unk464;
    u16 unk466;
} UnkStruct_022e9798;

extern s32 data_ov000_02369ac0[];

void func_ov000_022e9500(void);

void func_ov000_022e9798(UnkStruct_022e9798 *s) {
    s->unk464 = 6;
    s->unk466 = 1;
    data_ov000_02369ac0[1] = 0x25;
    s->func = func_ov000_022e9500;
}
