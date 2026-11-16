#include <nitro/types.h>

typedef struct UnkStruct_022d6e70 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x25a - 0x6c];
    u16 unk25A;
    u8 pad2[0x264 - 0x25c];
    u16 unk264;
    u8 pad3[0x2c4 - 0x266];
    u8 unk2C4[0x3d0 - 0x2c4];
    u8 unk3D0;
    u8 pad4[0x3d8 - 0x3d1];
    s32 unk3D8;
    u8 pad5[0x434 - 0x3dc];
    s32 unk434;
} UnkStruct_022d6e70;

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss3PartCC_State_Unk_ov000_022d6de4(void);

void func_ov000_022d6e70(UnkStruct_022d6e70 *s) {
    Model_SetAnimation(s->unk2C4, 0, s->unk434, 0xc, 0);
    s->unk3D0 &= ~4;
    s->unk3D8 = 0x1000;
    s->unk264 = 0x20;
    s->unk25A = 0;
    s->func = Boss3PartCC_State_Unk_ov000_022d6de4;
}
