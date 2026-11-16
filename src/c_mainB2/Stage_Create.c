#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_0204bf88;

typedef struct {
    u32 unk0;
    u32 unk4[3];
    s32 unk10;
    u32 pad14;
    u32 flags;
    u8 zone;
    u8 act;
    u8 pad1e[2];
    u32 unk20;
    u32 unk24;
} UnkState_0204bf88;

typedef struct {
    u8 pad0[0x46];
    u8 zone;
    u8 act;
    u32 unk48;
    u32 unk4c;
} UnkState2_0204bf88;

typedef struct {
    s16 v[9];
    s16 unk12;
    s16 unk14;
    s16 unk16;
} UnkCam_0204bf88;

typedef struct {
    s16 a;
    s16 b;
} UnkPair_0204bf88;

typedef struct {
    u8 pad0[0x9e];
    u8 unk9e;
} UnkGfx_0204bf88;

typedef struct {
    u32 flags;
} UnkG_0204bf88;

typedef s16 Tbl_0204bf88[9];

extern UnkState_0204bf88 gGameState;
extern UnkState2_0204bf88 data_022c4590;
extern u16 data_02074960[][3][3];
extern u16 data_02074962[][3][3];
extern u16 data_02074964[][3][3];
extern Tbl_0204bf88 data_020748bc[];
extern Tbl_0204bf88 data_020748be[];
extern Tbl_0204bf88 data_020748c0[];
extern Tbl_0204bf88 data_020748c2[];
extern Tbl_0204bf88 data_020748c4[];
extern Tbl_0204bf88 data_020748c6[];
extern Tbl_0204bf88 data_020748c8[];
extern Tbl_0204bf88 data_020748ca[];
extern Tbl_0204bf88 data_020748cc[];
extern UnkCam_0204bf88 data_022c4efc;
extern u16 data_02074a04[][3][8];
extern u16 data_022c4ee8;
extern UnkPair_0204bf88 data_022c4ef8;
extern u32 gObjManagerFlags;
extern u16 data_0207cf78;
extern UnkGfx_0204bf88 gCamera;
extern void *data_022c4ef4;
extern u8 data_ov000_0236a448[];
extern UnkG_0204bf88 gBossStatus;

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void NNS_G3dGlbLightColor(u32 index, u32 value);
void G3X_SetEdgeColorTable(const u16 *table);
void ObjDraw_InitRows(void);
void ObjDraw_SetManagedRows(u8 a, u8 b);
void func_ov000_02335db0(void);
void func_ov000_02335704(void);
void Camera_Create(void);
void Stage_InitMapCollision(void);
void func_ov000_0232c7b0(void);
void WirelessIcon_Create(void);
u8 WH_GetCurrentAid(void);
u16 NetPacket_FindSlot(u32 v);
void Player_Create(u16 id, u32 a);
void FixVsHud_Create(void);
void ScreenShake_Create(void);
void ObjRect_CheckInit(void);
void RingManager_Create(void);
void Hud_Create(void);
u32 Hud_GetFlags(void);
void Hud_SetFlags(u32 v);
void StageZoneScript_Create(void);
void BossCamera_Create(s32 a);
void ObjCollisionMap_CommitFrame(void);
void Stage_InitBlend(void);
void BrightnessFade_Create(s32 a, s32 b);
void StageTitleCard_Create(void);
void NaviMsg_Create(void);
void NaviMsg_SetVisible(BOOL on);
void NaviB_Create(void);
void Stage_Unk_0204ba84(void);
void Stage_Main(void);
void Stage_Destructor(void);

static inline BOOL IsBossAct_0204bf88(void)
{
    BOOL r = FALSE;
    if (IsBossAct_0204bf88())
        r = TRUE;
    return r;
}

static inline BOOL IsNormalAct_0204bf88(void)
{
    BOOL r = FALSE;
    if (gGameState.zone < 9 && gGameState.act < 2)
        r = TRUE;
    return r;
}

void Stage_Create(void) {
    UnkTask_0204bf88 *t;
    BOOL b;
    u16 i;
    u32 z;

    Task_Create(Stage_Unk_0204ba84, 0, 0x10, 0x10, 3, 4);
    t = Task_Create(Stage_Main, Stage_Destructor, 0, 0x4000, 3, 4);
    {
        vu16 z = 0;
        MIi_CpuClear16(z, t->work, 4);
    }
    NNS_G3dGlbLightColor(0, data_02074960[gGameState.zone][gGameState.act][0]);
    NNS_G3dGlbLightColor(1, data_02074962[gGameState.zone][gGameState.act][0]);
    NNS_G3dGlbLightColor(2, data_02074964[gGameState.zone][gGameState.act][0]);

    if (!IsNormalAct_0204bf88()) {
        data_022c4efc.v[0] = data_020748bc[gGameState.zone][0];
        data_022c4efc.v[1] = data_020748be[gGameState.zone][0];
        data_022c4efc.v[2] = data_020748c0[gGameState.zone][0];
        data_022c4efc.v[3] = data_020748c2[gGameState.zone][0];
        data_022c4efc.v[4] = data_020748c4[gGameState.zone][0];
        data_022c4efc.v[5] = data_020748c6[gGameState.zone][0];
        data_022c4efc.v[6] = data_020748c8[gGameState.zone][0];
        data_022c4efc.v[7] = data_020748ca[gGameState.zone][0];
        data_022c4efc.v[8] = data_020748cc[gGameState.zone][0];
    } else {
        data_022c4efc.v[0] = 0xb50;
        data_022c4efc.v[1] = -0xb50;
        data_022c4efc.v[2] = 0;
        data_022c4efc.v[3] = 0;
        data_022c4efc.v[4] = -0x617;
        data_022c4efc.v[5] = -0xecd;
        data_022c4efc.v[6] = 0xec7;
        data_022c4efc.v[7] = 0x620;
        data_022c4efc.v[8] = 0;
    }
    data_022c4efc.unk12 = 0xfff;
    data_022c4efc.unk14 = 0;
    data_022c4efc.unk16 = 0;
    data_022c4ee8 = 3;
    G3X_SetEdgeColorTable(data_02074a04[gGameState.zone][gGameState.act]);
    ObjDraw_InitRows();
    ObjDraw_SetManagedRows(3, 0x10);
    func_ov000_02335db0();
    func_ov000_02335704();
    Camera_Create();
    Stage_InitMapCollision();
    z = 0;
    gObjManagerFlags = z;
    data_022c4ef8.b = z;
    data_0207cf78 = 0x80;
    gObjManagerFlags |= 0x1c;
    data_022c4ef8.a = data_022c4ef8.b;
    if (IsBossAct_0204bf88()) {
        gObjManagerFlags |= 0x80;
        gObjManagerFlags &= ~4;
    } else {
        func_ov000_0232c7b0();
    }
    if ((gGameState.flags & 0x40) == 0 || ((gGameState.flags & 2) && gGameState.unk10 == 1)) {
        gGameState.unk20 = 0;
        gGameState.unk24 = 0;
    }
    if (gGameState.flags & 0x20) {
        WirelessIcon_Create();
        Player_Create(gGameState.unk0, NetPacket_FindSlot(WH_GetCurrentAid()));
        for (i = 0; i < 2; i++) {
            if (i != WH_GetCurrentAid())
                Player_Create(gGameState.unk4[i], NetPacket_FindSlot(i));
        }
        gCamera.unk9e = 1;
        gObjManagerFlags &= ~0x200;
        FixVsHud_Create();
    } else {
        Player_Create(gGameState.unk0, 0);
        gObjManagerFlags |= 0x200;
        if (gGameState.unk10 == 2 && data_022c4590.zone == gGameState.zone && data_022c4590.act == gGameState.act
            && data_022c4590.unk4c != 0) {
            gGameState.flags |= 0x80;
            Player_Create(data_022c4590.unk48, 1);
        }
    }
    ScreenShake_Create();
    ObjRect_CheckInit();
    RingManager_Create();
    Hud_Create();
    if (IsBossAct_0204bf88())
        Hud_SetFlags(Hud_GetFlags() | 4);
    if (gGameState.unk10 == 1 || gGameState.unk10 == 2)
        Hud_SetFlags(Hud_GetFlags() | 0x10);
    StageZoneScript_Create();
    BossCamera_Create(gGameState.act != 2 ? 0 : 2);
    data_022c4ef4 = data_ov000_0236a448;
    ObjCollisionMap_CommitFrame();
    ObjCollisionMap_CommitFrame();
    gGameState.flags &= ~1;
    Stage_InitBlend();
    gBossStatus.flags |= 1;
    b = FALSE;
    if (gGameState.zone >= 7 && gGameState.zone < 9 && gGameState.act < 2)
        b = TRUE;
    if (b) {
        BrightnessFade_Create(0x43, 0x100);
        gGameState.flags |= 1;
    } else if ((gGameState.flags & 8) == 0 && (gGameState.flags & 0x40) == 0) {
        StageTitleCard_Create();
    } else if (IsBossAct_0204bf88()) {
        NaviMsg_Create();
        NaviMsg_SetVisible(1);
        NaviB_Create();
        gGameState.flags |= 1;
    } else {
        gGameState.flags |= 1;
    }
    if (gGameState.flags & 0x40) {
        gGameState.flags &= ~0x40;
        BrightnessFade_Create(0x43, 0x100);
    }
}
