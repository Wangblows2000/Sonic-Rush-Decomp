#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 state;
    s32 unk8;
} UnkSub_022ca3c4;

typedef struct {
    u8 pad0[0x1c];
    s32 unk1c;
    u8 pad20[0x1ec - 0x20];
    UnkSub_022ca3c4 sub;
    u8 pad1f8[0x3ac - 0x1f8];
    void *work;
} UnkStruct_022ca3c4;

extern u8 gGameLanguage;

void OptionsMenu_DrawPageBackButton(UnkStruct_022ca3c4 *p, s16 x, s16 y, s32 a);
void OptionsMenu_DrawSprite(UnkStruct_022ca3c4 *p, s16 x, s16 y, s32 a, u16 b);
void OptionsMenu_DrawLinkLevelIcon(void *w, s16 x, s16 y);
void OptionsMenu_Unk_ov001_022c9924(UnkStruct_022ca3c4 *p, s16 x, s16 y);
void OptionsMenu_Unk_ov001_022c98a4(UnkStruct_022ca3c4 *p, s16 x, s16 y);
void OptionsMenu_DrawDownloadPlayChoiceLeft(UnkStruct_022ca3c4 *p, s16 x, s16 y);
void OptionsMenu_DrawDownloadPlayChoiceRight(UnkStruct_022ca3c4 *p, s16 x, s16 y);

#pragma thumb on
static inline u32 GetLang_022ca3c4(void) {
    u32 l;
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        l = gGameLanguage;
        break;
    default:
        l = 1;
        break;
    }
    return l;
}

void OptionsMenu_DrawPage5(UnkStruct_022ca3c4 *p, s16 x, s16 y) {
    void *w;
    UnkSub_022ca3c4 *s;

    if (x >= 0x100) return;
    w = p->work;
    s = &p->sub;
    if (s->state != 8) {
        if (p->unk1c >= 0) {
            OptionsMenu_DrawPageBackButton(p, x, y, 1);
        } else {
            OptionsMenu_DrawPageBackButton(p, x, y, 0);
        }
    }
    OptionsMenu_DrawSprite(p, x + 0x18, y + 0x18, 0, 0x1b);
    if (s->state >= 4 && s->state <= 8) {
        OptionsMenu_DrawLinkLevelIcon(w, x + 0x43, y + 0x46);
    }
    OptionsMenu_DrawSprite(p, x, y, 4, s->unk8 + 2);
    OptionsMenu_DrawSprite(p, x, y, 0xa, 0xd);
    if (GetLang_022ca3c4() == 0 || GetLang_022ca3c4() == 1) {
        OptionsMenu_DrawSprite(p, x, y, 0xb, 0xe);
    }
    OptionsMenu_DrawSprite(p, x, y, 3, 1);
    OptionsMenu_Unk_ov001_022c9924(p, x, y);
    OptionsMenu_Unk_ov001_022c98a4(p, x, y);
    OptionsMenu_DrawDownloadPlayChoiceLeft(p, x, y);
    OptionsMenu_DrawDownloadPlayChoiceRight(p, x, y);
}
#pragma thumb reset
