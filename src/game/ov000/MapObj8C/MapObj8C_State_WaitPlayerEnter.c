#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x64 - 4];
} UnkStruct_Plr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x32C - 0x20C];
    u32 unk32C;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    s8 unk210[2];
    u8 pad212[0x216 - 0x212];
    u16 unk216;
    u16 unk218;
    u16 unk21A;
    u16 unk21C;
} UnkStruct_02309850;

extern UnkStruct_Plr gCamera[];
extern UnkStruct_Player *gPlayers[];

void MapObj8C_Action_StartRoom(UnkStruct_02309850 *w);

void MapObj8C_State_WaitPlayerEnter(UnkStruct_02309850 *w) {
    UnkStruct_Player *p;
    u8 i;

    if (!(w->unk20C & 0x200) && w->unk216 != 0xffff && w->unk218 != 0xffff && w->unk21A != 0xffff && w->unk21C != 0xffff)
        w->unk20C |= 0x200;
    if (!(w->unk20C & 0x200))
        return;
    for (i = 0; i < 2; i = (u8)(i + 1)) {
        if (w->unk210[i] >= 0) {
            p = gPlayers[w->unk210[i]];
            if (p->base.unk18 == 0) {
                s32 x = p->base.unk10;
                s32 y = p->base.unk14;
                if (w->unk216 << 8 <= x && x <= w->unk21A << 8 && w->unk218 << 8 <= y && y <= w->unk21C << 8 &&
                    ((gCamera[2].unk0 & 1) || !(p->unk32C & 0x400))) {
                    if (x < w->base.unk10) {
                        if (w->unk20C & 1) {
                            MapObj8C_Action_StartRoom(w);
                            return;
                        }
                    } else if (!(w->unk20C & 1)) {
                        MapObj8C_Action_StartRoom(w);
                        return;
                    }
                }
            }
        }
    }
}
