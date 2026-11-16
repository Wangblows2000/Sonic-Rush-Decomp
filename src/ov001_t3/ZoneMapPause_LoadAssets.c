#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1a;
} UnkRes_022e9408;

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022e9408;

typedef struct {
    u8 pad0[0x14];
    u8 unk14[0x54 - 0x14];
    UnkSpr_022e9408 spr[4];
    s32 anim[4];
    UnkRes_022e9408 *res[3];
} UnkStruct_022e9408;

extern u8 gGameLanguage;
extern char **gZoneMapPauseFileTable[];

UnkRes_022e9408 *File_Load(char *name, s32 v);
void Background_Init(void *o, UnkRes_022e9408 *r, s32 a, s32 b, s32 c, u16 d, u16 e);
void Background_Draw(void *o);
void ZoneMapPause_Unk_ov001_022e93b0(UnkSpr_022e9408 *s, UnkRes_022e9408 *r, s32 a, s32 b, s32 c);

#pragma thumb on
void ZoneMapPause_LoadAssets(UnkStruct_022e9408 *p) {
    u32 lang;
    char **names;
    s32 i;
    UnkRes_022e9408 *r;

    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        lang = gGameLanguage;
        break;
    default:
        lang = 1;
        break;
    }
    names = gZoneMapPauseFileTable[lang];
    for (i = 0; i < 3; i++) {
        p->res[i] = File_Load(names[i], 0);
    }
    r = p->res[0];
    Background_Init(p->unk14, r, 0, 0, 2, r->unk18, r->unk1a);
    Background_Draw(p->unk14);
    r = p->res[2];
    ZoneMapPause_Unk_ov001_022e93b0(&p->spr[0], r, 0, 0xe, 4);
    p->anim[0] = -1;
    ZoneMapPause_Unk_ov001_022e93b0(&p->spr[1], r, 2, 0xe, 4);
    p->anim[1] = -1;
    ZoneMapPause_Unk_ov001_022e93b0(&p->spr[2], r, 5, 0xe, 4);
    p->anim[2] = -1;
    ZoneMapPause_Unk_ov001_022e93b0(&p->spr[3], p->res[1], 0, 0xf, 0);
    p->anim[3] = -1;
}
#pragma thumb reset
