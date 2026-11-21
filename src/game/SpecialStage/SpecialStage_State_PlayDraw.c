#include <nitro/types.h>

typedef struct UnkStruct_0230ba9c_t {
    u32 v[8];
} UnkStruct_0230ba9c_t;

typedef struct UnkStruct_0230ba9c {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[8];
    s32 timer;
} UnkStruct_0230ba9c;

extern UnkStruct_0230ba9c *gSpecialStageWork;
extern UnkStruct_0230ba9c_t data_ov001_0231e690;

void SpecialStageCamera_Apply(void);
void SpecialStageMap_Draw(void);
void SpecialStageGimmick_Draw(s32 a);
void SpecialStage_DrawObjects(void);
void SpecialStagePlayer_Draw(void);
void SpecialStageHud_Draw(void);
void G3X_SetFog(s32 a, s32 b, s32 c, s32 d);
void NNS_G3dGlbPolygonAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void G3X_SetFogTable(UnkStruct_0230ba9c_t *t);

void SpecialStage_State_PlayDraw(void) {
    UnkStruct_0230ba9c *g = gSpecialStageWork;
    UnkStruct_0230ba9c_t tbl;
    s32 i;
    u32 v;

    SpecialStageCamera_Apply();
    SpecialStageMap_Draw();
    SpecialStageGimmick_Draw(g->unk1C);
    SpecialStage_DrawObjects();
    if (g->unk18 != 0)
        SpecialStagePlayer_Draw();
    SpecialStageHud_Draw();
    if (g->timer < 0x78) {
        tbl = data_ov001_0231e690;
        NNS_G3dGlbPolygonAttr(0, 0, 3, 0, 0x1f, 0x8000);
        for (i = 0; i < 8; i++) {
            v = (0x7f - g->timer * 0x7f / 120) & 0x7f;
            tbl.v[i] = v | ((v << 24) | (v << 16) | (v << 8));
        }
        G3X_SetFog(1, 0, 4, 0);
        *(vu32 *)0x4000358 = 0x1f7fff;
        G3X_SetFogTable(&tbl);
    } else {
        G3X_SetFog(0, 0, 4, 0);
    }
}
