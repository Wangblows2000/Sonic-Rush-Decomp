#include <nitro/types.h>

extern u8 data_ov001_02316840[];
extern void *data_ov001_0231f074;
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void ExtraBoss_Unk_ov001_022fa908(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void ExtraBossObj_State_Unk_ov001_022f1660(void);
void ExtraBossObj_State_Unk_ov001_022f0e4c(void);

void ExtraBoss_Unk_ov001_022eed64(u8 **pa, u8 **pb) {
    u8 *a = *pa;
    u8 *b = *pb;
    u8 *c = *(u8 **)(a + 0x7c);
    u8 *d = *(u8 **)(c + 0x1dc);
    u8 *e;
    u8 *f;
    if (*(s32 *)(b + 4) != 0) {
        return;
    }
    e = *(u8 **)(a + 0x84);
    f = *(u8 **)(b + 0x84);
    *(u16 *)(f + 8) = 0x1d;
    f[0xa] = 4;
    f[0xb] = 0xc;
    f[0xc] = 2;
    *(s32 *)(b + 0x18) = 0;
    *(s32 *)(b + 0x1c) = 0x14000;
    *(s32 *)(b + 0x20) = 0;
    *(u16 *)(d + 0x16) = 1;
    *(void (**)(void))(c + 0x74) = ExtraBossObj_State_Unk_ov001_022f1660;
    *(void (**)(void))(a + 0x74) = ExtraBossObj_State_Unk_ov001_022f0e4c;
    Model_SetAnimation(e, 4, data_ov001_0231f074, *(u16 *)(data_ov001_02316840 + 0x76), 0);
    e[0x110] &= ~4;
    *(s32 *)(e + 0x114) = 0x1000;
    *(s32 *)(e + 0x118) = 0;
    *(u32 *)(a + 0x128) |= 0x200;
    ExtraBoss_Unk_ov001_022fa908(0, *(s32 *)(d + 0x190), -*(s32 *)(d + 0x194), 0);
    Sound_PlaySeqArc(0, 0, 0xb5);
}
