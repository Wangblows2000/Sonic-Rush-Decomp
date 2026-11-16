#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 flags;
    u32 pad1c;
    s32 unk20;
    s32 unk24;
} UnkState_0204b95c;

typedef struct {
    u8 pad0[0x14];
    u8 unk14;
} UnkState2_0204b95c;

typedef struct {
    u8 pad0[0x350];
    s16 unk350;
    u8 pad352[0x5c];
    s16 unk3ae;
} UnkPlayer_0204b95c;

typedef struct {
    u8 pad0[0x18];
    s16 unk18;
    s16 unk1a;
    u8 pad1c[0x60];
    s16 unk7c;
    s16 unk7e;
} UnkGfx_0204b95c;

extern UnkState2_0204b95c data_022c4590;
extern UnkState_0204b95c gGameState;
extern UnkPlayer_0204b95c *gPlayers[];
extern u32 gObjManagerFlags;
extern UnkGfx_0204b95c gCamera;
extern u16 gPad[];

void Hud_Unk_ov000_023375f8(u8 a);
void Hud_SetRingCount(s32 a);
void Hud_SetTime(s32 a);
void Hud_SetTensionGauge(u16 a, s32 b);
s32 ScreenShake_GetOffsetX(void);
s32 ScreenShake_GetOffsetY(void);
void ObjDraw_SetGlobalOffset(s32 a, s32 b);
void ObjRect_CheckAllGroup(void);
void ObjCollisionMap_CommitFrame(void);
void PauseMenu_Create(s32 mode);
void NetPacket_BuildSendBuffer(void);

void Stage_Main(void) {
    Hud_Unk_ov000_023375f8(data_022c4590.unk14);
    Hud_SetRingCount(gPlayers[0]->unk3ae);
    Hud_SetTime(gGameState.unk20);
    if (gGameState.flags & 1)
        gGameState.unk20++;
    gGameState.unk24++;
    Hud_SetTensionGauge((u16)(gPlayers[0]->unk350 >> 4), 0);
    if ((gObjManagerFlags & 0x80) == 0) {
        ObjDraw_SetGlobalOffset(ScreenShake_GetOffsetX(), ScreenShake_GetOffsetY());
        gCamera.unk18 = -ScreenShake_GetOffsetX();
        gCamera.unk1a = -ScreenShake_GetOffsetY();
        gCamera.unk7c = gCamera.unk18;
        gCamera.unk7e = gCamera.unk1a;
    }
    ObjRect_CheckAllGroup();
    ObjCollisionMap_CommitFrame();
    if (gPad[2] & 8)
        PauseMenu_Create(0);
    if (gGameState.flags & 0x20)
        NetPacket_BuildSendBuffer();
}
