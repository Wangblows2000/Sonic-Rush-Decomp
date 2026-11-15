#include <nitro/types.h>

typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02054a04;
typedef struct { u16 a; u16 b; } UnkPair_02054a04;
typedef struct {
    UnkPair_02054a04 p[4]; /* 0x0 */
    u8 pad10[0xc];
    s32 unk1C;
    u16 unk20;
    u8 pad22[0x28];
    u16 unk4A;
} UnkBg_02054a04;
extern UnkBlend_02054a04 data_0208926c;
extern UnkBlend_02054a04 data_02087d44;
extern u32 data_02087cb8;
extern UnkBg_02054a04 *data_02072c78[];

void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void func_02037134(void);
void func_02035368(void);
void func_02035cb4(void);
void func_02033754(void);
void func_02039d08(u16 a);
void func_02039b48(void);
void func_020395f4(void);
void func_02034bc0(void);
void func_02034b18(void);
void func_02034a70(void);

#pragma thumb on
void func_02054a04(void) {
    UnkBg_02054a04 *s;
    u16 i;
    u16 j;

    data_0208926c.unk8 = -0x10;
    data_02087d44.unk8 = -0x10;
    GXx_SetMasterBrightness_((vu16 *)0x0400006c, data_0208926c.unk8);
    GXx_SetMasterBrightness_((vu16 *)0x0400106c, data_02087d44.unk8);
    func_02037134();
    func_02035368();
    func_02035cb4();
    func_02033754();
    data_02087cb8 &= ~0xc00;
    for (i = 0; i < 2; i++) {
        s = data_02072c78[i];
        for (j = 0; j < 4; j++)
            s->p[j].a = s->p[j].b = 0;
        s->unk1C = 0;
        s->unk20 &= ~0xc0;
        s->unk4A = 0;
        func_02039d08(i);
        func_02039d08(i);
    }
    func_02039b48();
    func_020395f4();
    func_02034bc0();
    func_02034b18();
    func_02034a70();
}
#pragma thumb reset
