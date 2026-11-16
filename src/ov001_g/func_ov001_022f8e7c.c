#include <nitro/types.h>
#pragma opt_propagation off

typedef struct UnkStruct_022f8e7c {
    u8 pad0[0xe10];
    void *task;
    u8 padE14[0xe48 - 0xe14];
    s16 unkE48;
} UnkStruct_022f8e7c;

extern UnkStruct_022f8e7c *data_ov001_0231f084;
extern vu8 data_0208923c[];

void *func_020372d0(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void func_ov001_022f8c78(void);
void func_020367b4(s32 a, s32 b);

void func_ov001_022f8e7c(void) {
    vu8 *w;

    data_ov001_0231f084->unkE48 = 0;
    w = data_0208923c;
    data_ov001_0231f084->task = func_020372d0(func_ov001_022f8c78, 0, 0, 0x2000, 0, 0);
    w[1] = 0;
    w[5] = 0;
    w[0] = 0x80;
    w[4] = 0xc0;
    w[3] = 0x7f;
    w[7] = 0;
    w[2] = 0;
    w[6] = 0xc0;
    w[8] = 1;
    w[8] |= 0x10;
    w[9] = 1;
    w[9] |= 0x10;
    w[10] = 2;
    w[10] |= 0x10;
    *(vu32 *)(w + 0xc) |= 3;
    func_020367b4(0, 0x12);
}
