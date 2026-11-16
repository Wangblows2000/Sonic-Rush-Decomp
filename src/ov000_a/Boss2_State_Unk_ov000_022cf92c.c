#include <nitro/types.h>

typedef struct UnkStruct_022cf92c {
    u8 pad[0x12f4];
    s32 unk12F4;
} UnkStruct_022cf92c;

typedef struct UnkStruct_022cf92c_o {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[4];
    s32 unk18;
} UnkStruct_022cf92c_o;

typedef struct UnkStruct_022cf92c_4 {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022cf92c_4;

typedef struct UnkStruct_022cf92c_5 {
    u8 pad[0x3ae];
    s16 unk3AE;
} UnkStruct_022cf92c_5;

typedef struct UnkStruct_022cf92c_6 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022cf92c_6;

typedef struct UnkStruct_022cf92c_7 {
    u8 pad[0x62];
    u16 unk62;
    u8 pad1[0xe0 - 0x64];
    s32 unkE0;
    s32 unkE4;
} UnkStruct_022cf92c_7;

extern UnkStruct_022cf92c_4 gDisplayMain;
extern UnkStruct_022cf92c_5 *gPlayers;
extern UnkStruct_022cf92c_6 data_022c4590;
extern UnkStruct_022cf92c_7 gCamera;

void Stage_OnBossCleared(void);
void StageShutdown_Create(void (*f)(void));
UnkStruct_022cf92c_o *func_ov000_023293f4(UnkStruct_022cf92c *s, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
UnkStruct_022cf92c_o *func_ov000_023295a4(UnkStruct_022cf92c *s, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Boss2_State_Unk_ov000_022cf92c(UnkStruct_022cf92c *s) {
    UnkStruct_022cf92c_o *a;
    UnkStruct_022cf92c_o *b;
    u16 h;
    s->unk12F4--;
    if ((s->unk12F4 & 3) == 0) {
        if (gDisplayMain.unk48 > 0)
            gDisplayMain.unk48--;
    }
    if (s->unk12F4 <= 0) {
        data_022c4590.unk10 = gPlayers->unk3AE * 1000;
        StageShutdown_Create(Stage_OnBossCleared);
        return;
    }
    if (s->unk12F4 != 0xd7)
        return;
    h = gCamera.unk62;
    if (!(h <= 502.1))
        return;
    a = func_ov000_023293f4(s, 0, 1, 1, h, gCamera.unkE0, gCamera.unkE4);
    b = func_ov000_023295a4(s, 0, 0, gCamera.unk62, 1, gCamera.unkE0, gCamera.unkE4);
    a->unk10 = b->unk10 = -0x6400;
    a->unk18 = b->unk18 = 0;
    a = func_ov000_023293f4(s, 0, 1, 1, gCamera.unk62, gCamera.unkE0, gCamera.unkE4);
    b = func_ov000_023295a4(s, 0, 0, gCamera.unk62, 1, gCamera.unkE0, gCamera.unkE4);
    a->unk10 = b->unk10 = 0x6400;
    a->unk18 = b->unk18 = 0;
    a = func_ov000_023293f4(s, 0, 1, 2, gCamera.unk62, gCamera.unkE0, gCamera.unkE4);
    b = func_ov000_023295a4(s, 0, 0, gCamera.unk62, 1, gCamera.unkE0, gCamera.unkE4);
    a->unk10 = b->unk10 = 0x6400;
    a->unk18 = b->unk18 = 0x6400;
}
