#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2C];
    u32 flags;
    u8 pad30[0x46 - 0x30];
    u16 unk46;
} UnkStruct_SSSprite;

typedef struct {
    u8 pad0[0x8];
    u8 bg0[0x40];
    u8 bg1[0x40];
    u8 pad88[0x780 - 0x88];
    UnkStruct_SSSprite spr0;
    UnkStruct_SSSprite spr1;
    UnkStruct_SSSprite spr2[10];
    u32 padAE0;
    s32 ringQuota;
    s32 unkAE8;
    u8 padAEC[0xBC0 - 0xAEC];
    s32 unkBC0;
} UnkStruct_SSHud;

typedef struct {
    u8 pad0[0x18];
    u32 offset;
} UnkStruct_SSFile;

typedef struct {
    u8 pad0[0x1C];
    UnkStruct_SSFile *file1C;
    UnkStruct_SSFile *file20;
    UnkStruct_SSFile *file24;
} UnkStruct_SSAssets;

extern UnkStruct_SSHud *gSpecialStageHud;
extern UnkStruct_SSAssets *data_ov001_0231f5c4;
extern u16 data_ov001_023170b8[];

void *ObjVram_Alloc(u32 a, u32 b);
void AnimatorSprite_Init(void *s, void *h, u16 idx, u32 d, u32 e, u32 f, void *g, u32 hh, u32 i);
void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);
void Background_Draw(void *bg);
s32 SpecialStage_GetRingQuota(void);

void SpecialStage_Unk_ov001_023099ec(void)
{
    UnkStruct_SSHud *hud;
    UnkStruct_SSFile *file;
    void *vram;
    s32 i;
    UnkStruct_SSSprite *spr;
    UnkStruct_SSHud *h;
    u16 anim;

    hud = gSpecialStageHud;
    hud->unkAE8 = 1;

    file = data_ov001_0231f5c4->file20;
    vram = ObjVram_Alloc(0, *(u16 *)((u8 *)file + file->offset + 0x30));
    AnimatorSprite_Init(&hud->spr0, data_ov001_0231f5c4->file20, 1, 0, 0, 0, vram, 0, 0x05000200);
    AnimatorSprite_ProcessAnimation(&hud->spr0, 0, 0);
    hud->spr0.flags |= 0x10;
    hud->spr0.unk46 = 1;

    file = data_ov001_0231f5c4->file24;
    vram = ObjVram_Alloc(0, *(u16 *)((u8 *)file + file->offset + 6));
    AnimatorSprite_Init(&hud->spr1, data_ov001_0231f5c4->file24, 0, 0, 0, 0, vram, 0, 0x05000200);
    AnimatorSprite_ProcessAnimation(&hud->spr1, 0, 0);
    hud->spr1.unk46 = 1;
    hud->unkBC0 = 0x1000;

    for (i = 0, spr = hud->spr2, h = hud; i < 10; spr++, h = (UnkStruct_SSHud *)((u8 *)h + 0x48), i++) {
        anim = data_ov001_023170b8[i];
        file = data_ov001_0231f5c4->file1C;
        vram = ObjVram_Alloc(0, *(u16 *)((u8 *)file + file->offset + anim * 0x14 + 0x1C));
        AnimatorSprite_Init(spr, data_ov001_0231f5c4->file1C, anim, 0, 0, 0, vram, 0, 0x05000200);
        AnimatorSprite_ProcessAnimation(spr, 0, 0);
        h->spr2[0].flags |= 0x10;
        h->spr2[0].unk46 = 5;
    }
    *(u16 *)((u8 *)hud + 0xA54) = 0x20;
    *(u16 *)((u8 *)hud + 0xA56) = 0xB4;
    *(u32 *)((u8 *)hud + 0xAC4) |= 4;
    Background_Draw(hud->bg0);
    Background_Draw(hud->bg1);
    hud->ringQuota = SpecialStage_GetRingQuota();
}
