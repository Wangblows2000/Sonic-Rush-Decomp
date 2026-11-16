#include <nitro/types.h>

typedef struct UnkStruct_02309228_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 0x8];
    u32 unk2C;
    u8 pad30[0x48 - 0x30];
} UnkStruct_02309228_o;

typedef struct UnkStruct_02309228 {
    void *unk0;
    void *unk4;
    u8 pad8[0x108 - 0x8];
    UnkStruct_02309228_o objs[0x19];
    u8 pad810[0xaf4 - 0x810];
    float fA[7];
    float fB[7];
    float fC[7];
    u8 padB48[0xbb0 - 0xb48];
    s32 unkBB0;
    u8 padBB4[0xbc0 - 0xbb4];
    s32 unkBC0;
    s32 timer;
} UnkStruct_02309228;

extern UnkStruct_02309228 *gSpecialStageHud;
extern s16 gDisplaySub[];
extern s16 data_ov001_02317098[];
extern u16 data_ov001_02317114[];

void SpecialStageMap_SetScrollSpeed(s32 v);
BOOL Runtime_Unk_0202e564(float a, float b);
void AnimatorSprite_SetAnimation(UnkStruct_02309228_o *s, u16 index);
s32 SpecialStage_GetStageIndex(void);
s32 SpecialStage_GetPlayer(void);
void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void SpecialStageHud_Unk_ov001_0230956c(void);
void SpecialStageHud_Unk_ov001_023095e0(void);
void SpecialStageCamera_SetMode(s32 v);
void SpecialStage_Unk_ov001_02312dd0(s32 p, s32 v);
void SpecialStageHud_Unk_ov001_02312dd8(s32 p, s32 v);

void SpecialStageHud_Unk_ov001_02309228(void) {
    UnkStruct_02309228 *g = gSpecialStageHud;
    s32 i;
    s32 t;
    s16 tgt;
    s32 d;
    UnkStruct_02309228_o *o;

    g->timer++;
    if (g->timer < 0x118)
        SpecialStageMap_SetScrollSpeed(9216.0f * g->timer / 280.0f);
    if (g->timer <= 0x10)
        gDisplaySub[0x24] = 0x10 - g->timer;
    if (g->timer >= 0x96 && g->timer < 0x136) {
        t = g->timer - 0x96;
        for (i = 0; i < 7; i++) {
            tgt = data_ov001_02317098[i];
            o = &g->objs[i + 0xb];
            d = tgt - g->objs[i + 0xb].x;
            if (t > (6 - i) * 10) {
                if (d <= 0) {
                    o->x = tgt;
                } else if (Runtime_Unk_0202e564(g->fA[i], 0.0f)) {
                    g->fA[i] = 0;
                    g->fB[i] = 0;
                    o->x = tgt;
                } else {
                    o->x = g->fC[i];
                    g->fC[i] += g->fA[i];
                    g->fA[i] += g->fB[i];
                }
            }
            if (g->timer == i + 0x10e) {
                AnimatorSprite_SetAnimation(o, (u8)(data_ov001_02317114[i + 0xb] + 1));
            } else if (g->timer > i + 0x10e && (o->unk2C & 2)) {
                AnimatorSprite_SetAnimation(o, (u8)data_ov001_02317114[i + 0xb]);
            }
        }
    }
    if (g->timer < 0x96)
        return;
    g->objs[0x17].x = 0x80;
    g->objs[0x17].y = 0x22;
    g->objs[0x18].x = 0x80;
    g->objs[0x18].y = 0x2a;
    switch (g->timer) {
    case 0x96:
        AnimatorSprite_SetAnimation(&g->objs[0x17], SpecialStage_GetStageIndex() * 2 + 1);
        g->unk0 = Task_Create(SpecialStageHud_Unk_ov001_0230956c, 0, 0, 0xf300, 0, 0);
        g->unkBB0 = 1;
        break;
    case 0x118:
        AnimatorSprite_SetAnimation(&g->objs[0x18], 2);
        g->unkBC0 = 0;
        SpecialStageCamera_SetMode(7);
        g->unk4 = Task_Create(SpecialStageHud_Unk_ov001_023095e0, 0, 0, 0xf200, 0, 0);
        SpecialStageMap_SetScrollSpeed(0x3000);
        SpecialStage_Unk_ov001_02312dd0(SpecialStage_GetPlayer(), 0x50);
        break;
    case 0x12c:
        SpecialStageCamera_SetMode(8);
        break;
    case 0x168:
        SpecialStageCamera_SetMode(0);
        SpecialStageHud_Unk_ov001_02312dd8(SpecialStage_GetPlayer(), 1);
        g->timer = -1;
        break;
    }
    if (g->timer <= 0x118)
        return;
    g->unkBC0 += 0x200;
    if (g->unkBC0 >= 0x1000)
        g->unkBC0 = 0x1000;
}
