#include <nitro/types.h>
#include <game/Graphics/animatorsprite.h>

typedef struct {
    u8 pad0[0x14];
    UnkSpr_W50 spr[7]; /* 0x14 */
    u8 pad20C[0x4];
    void *res1;        /* 0x210 */
    void *res2;        /* 0x214 */
} UnkWork_0205788c;

extern u8 data_0207c5d4[];
extern u8 gGameLanguage;
extern void *data_0207c490[];

void *File_Load(void *a, u32 b);
void *ObjVram_Alloc(u32 a, u32 b);
void AnimatorSprite_Init(UnkSpr_W50 *s, void *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, u32 h);
void AnimatorSprite_ProcessAnimation(UnkSpr_W50 *s, u32 a, u32 b);

#pragma thumb on
void PolyDemoUnder2_LoadSprites(UnkWork_0205788c *w) {
    UnkSpr_W50 *spr;
    u32 idx;

    w->res1 = File_Load(data_0207c5d4, 0);
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        idx = gGameLanguage;
        break;
    default:
        idx = 1;
        break;
    }
    w->res2 = File_Load(data_0207c490[idx], 0);

    spr = &w->spr[0];
    AnimatorSprite_Init(spr, w->res1, 0, 4, 1, 0, ObjVram_Alloc(1, 0x20), 0, 0x05000600);
    spr->unk44 = 3;
    spr->unk46 = 0;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->unk2C |= 0x10;

    spr = &w->spr[1];
    AnimatorSprite_Init(spr, w->res1, 1, 4, 1, 0, ObjVram_Alloc(1, 0x20), 0, 0x05000600);
    spr->unk44 = 3;
    spr->unk46 = 0;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->unk2C |= 0x10;

    spr = &w->spr[2];
    AnimatorSprite_Init(spr, w->res1, 2, 4, 1, 0, ObjVram_Alloc(1, 0x20), 0, 0x05000600);
    spr->unk2C |= 0x10;
    spr->unk44 = 0;
    spr->unk46 = 1;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);

    spr = &w->spr[3];
    AnimatorSprite_Init(spr, w->res1, 3, 4, 1, 0, ObjVram_Alloc(1, 0x20), 0, 0x05000600);
    spr->unk2C |= 0x10;
    spr->unk44 = 0;
    spr->unk46 = 1;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);

    spr = &w->spr[4];
    AnimatorSprite_Init(spr, w->res2, 0, 4, 1, 0, ObjVram_Alloc(1, 0x20), 0, 0x05000600);
    spr->unk44 = 2;
    spr->unk46 = 0;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->unk2C |= 0x10;

    spr = &w->spr[5];
    AnimatorSprite_Init(spr, w->res2, 1, 4, 1, 0, ObjVram_Alloc(1, 0x20), 0, 0x05000600);
    spr->unk44 = 0;
    spr->unk46 = 0;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->unk2C |= 0x10;

    spr = &w->spr[6];
    AnimatorSprite_Init(spr, w->res2, 2, 4, 1, 0, ObjVram_Alloc(1, 0x20), 0, 0x05000600);
    spr->unk44 = 0;
    spr->unk46 = 0;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr->unk2C |= 0x10;
}
#pragma thumb reset
