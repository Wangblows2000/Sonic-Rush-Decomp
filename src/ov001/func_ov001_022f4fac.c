#include <nitro/types.h>

typedef struct {
    s32 w0;
    u8 pad4[0xc];
    s32 w10;
    s32 w14;
    s32 w18;
    u8 b1c;
    u8 b1d;
    u16 h1e;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern void *data_ov001_0231f068;
extern s32 data_ov001_0231694c[][5];
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
u8 *func_ov001_022ffce8(void);
u16 func_02002094(s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov001_022f4f54(void);

void func_ov001_022f4fac(u8 *p) {
    u8 *q = *(u8 **)(p + 0x84);
    BOOL b;
    s32 k;
    *(void (**)(void))(p + 0x74) = func_ov001_022f4f54;
    Model_SetAnimation(q, 0, data_ov001_0231f068, 0x13, 0);
    q[0x10c] &= ~4;
    *(s32 *)(q + 0x114) = 0x1000;
    b = TRUE;
    if (data_022c4560.w10 != 2 && data_022c4560.w10 != 3) {
        b = FALSE;
    }
    if (b) {
        k = 1;
    } else {
        k = data_022c4560.w14;
    }
    *(s32 *)(p + 0x1cc) = data_ov001_0231694c[k][*(s32 *)(func_ov001_022ffce8() + 0x40)];
    *(u16 *)(p + 0x1d4) = func_02002094(0x5555, *(s32 *)(p + 0x1cc) - 1);
    *(u32 *)(p + 0xb8) |= 0x200;
    Sound_PlaySeqArc(0, 0, 0xa5);
}
