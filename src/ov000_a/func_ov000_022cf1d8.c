#include <nitro/types.h>

typedef struct UnkStruct_022cf1d8_a {
    u8 pad[0x1c];
    s32 unk1C;
    u8 pad1[0x4c - 0x20];
    s32 unk4C;
    u8 pad2[0x10c - 0x50];
    u8 unk10C[4];
    u8 pad3[0x114 - 0x110];
    s32 unk114;
    u8 pad4[0x138 - 0x118];
} UnkStruct_022cf1d8_a;

typedef struct UnkStruct_022cf1d8_s {
    u8 pad[0x5bc];
    UnkStruct_022cf1d8_a unk5BC;
    UnkStruct_022cf1d8_a unk6F4;
    UnkStruct_022cf1d8_a unk82C;
    u8 pad2[0xf7c - 0x82c - 0x138];
    s32 unkF7C;
    s32 unkF80;
    u8 pad3[0x1420 - 0xf84];
    UnkStruct_022cf1d8_a *unk1420;
    u8 pad4[0x1454 - 0x1424];
    s32 unk1454;
} UnkStruct_022cf1d8_s;

typedef struct UnkStruct_022cf1d8 UnkStruct_022cf1d8;
struct UnkStruct_022cf1d8 {
    UnkStruct_022cf1d8_s *unk0;
    void (*func)(UnkStruct_022cf1d8 *t);
    s32 unk8;
    s32 unkC;
};

typedef struct UnkStruct_022cf1d8_cam {
    u8 pad[0x3c];
    s32 unk3C;
    s32 unk40;
} UnkStruct_022cf1d8_cam;

extern UnkStruct_022cf1d8_cam data_ov000_0236a444;

s32 FX_DivS32(s32 a, s32 b);
void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);

s32 ScreenShake_GetOffsetX(void);
s32 ScreenShake_GetOffsetY(void);
void ScreenShake_Start(s32 a);
void func_ov000_022cf118(UnkStruct_022cf1d8 *t);
void func_ov000_022cd500(void);

void func_ov000_022cf1d8(UnkStruct_022cf1d8 *t) {
    t->unk8 += 0x1b33;
    if (t->unk8 > 0x66000)
        t->unk8 = 0x66000;
    data_ov000_0236a444.unk3C = ScreenShake_GetOffsetX() * 0xccc;
    data_ov000_0236a444.unk40 = ScreenShake_GetOffsetY() * 0xccc;
    t->unkC--;
    if (t->unkC == 0) {
        t->func = func_ov000_022cf118;
        ScreenShake_Start(0xc);
        t->unkC = 0xf;
    } else if (t->unkC == 0x76) {
        func_ov000_022cd500();
    }
}
