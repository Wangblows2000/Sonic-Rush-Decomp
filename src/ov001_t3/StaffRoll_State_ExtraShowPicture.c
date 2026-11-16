#include <nitro/types.h>

typedef struct {
    u16 bg0a : 1;
    u16 bg1a : 1;
    u16 bg2a : 1;
    u16 bg3a : 1;
    u16 obja : 1;
    u16 bda : 1;
    u16 mode : 2;
    u16 bg0b : 1;
    u16 bg1b : 1;
    u16 bg2b : 1;
    u16 bg3b : 1;
    u16 objb : 1;
    u16 bdb : 1;
    u16 pad : 2;
} UnkBlend_022d1904;

typedef struct {
    u16 eva : 5;
    u16 pad0 : 3;
    u16 evb : 5;
    u16 pad1 : 3;
} UnkAlpha_022d1904;

typedef struct {
    u8 pad0[0x20];
    UnkBlend_022d1904 blend;
    UnkAlpha_022d1904 alpha;
} UnkGfx_022d1904;

typedef struct {
    u8 pad0[0x40];
} UnkObj_022d1904;

typedef struct {
    u8 pad0[0x54];
    UnkObj_022d1904 objs[(0xd08 - 0x54) / 0x40];
    u8 pad[0xd08 - 0x54 - ((0xd08 - 0x54) / 0x40) * 0x40];
    s32 unkd08;
    u8 unkd0c[0xf0c - 0xd0c];
    u8 unkf0c[4];
} UnkStruct_022d1904;

extern UnkGfx_022d1904 gDisplayMain;

void StaffRoll_Unk_ov001_022d0070(UnkStruct_022d1904 *p);
void StaffRoll_Unk_ov001_022cff74(UnkObj_022d1904 *o, s32 a, void *b, void *c);
void StaffRoll_SetState(UnkStruct_022d1904 *p, void (*fn)(UnkStruct_022d1904 *));
void StaffRoll_State_ExtraPictureFadeIn(UnkStruct_022d1904 *p);

#pragma thumb on
static inline u32 GetPlane_022d1904(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetPlane_022d1904(u32 plane) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (plane << 8);
}

void StaffRoll_State_ExtraShowPicture(UnkStruct_022d1904 *p) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1a00;
    StaffRoll_Unk_ov001_022d0070(p);
    gDisplayMain.blend.bg0a = 0;
    gDisplayMain.blend.bg1a = 0;
    gDisplayMain.blend.bg2a = 0;
    gDisplayMain.blend.bg3a = 0;
    gDisplayMain.blend.bg0b = 1;
    gDisplayMain.blend.bg1b = 1;
    gDisplayMain.blend.bg2b = 1;
    gDisplayMain.blend.bg3b = 1;
    gDisplayMain.blend.mode = 0;
    gDisplayMain.alpha.eva = 0;
    gDisplayMain.alpha.evb = 15 - gDisplayMain.alpha.eva;
    SetPlane_022d1904(GetPlane_022d1904() | 1);
    StaffRoll_Unk_ov001_022cff74(&p->objs[p->unkd08 + 0xb], 0x100, p->unkf0c, p->unkd0c);
    StaffRoll_SetState(p, StaffRoll_State_ExtraPictureFadeIn);
}
#pragma thumb reset
