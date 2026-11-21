#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    u8 pad8[8];
} UnkNode_022e8090;

typedef struct {
    u8 pad0[4];
    u8 sub[0x670];
    UnkNode_022e8090 nodes[(0x1994 - 0x674) / 16];
    u8 obj1994[0x1cc4 - 0x1994];
    s32 unk1cc4;
} UnkMap_022e8090;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    u32 flags;
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    s32 unk20;
    u8 pad24[0x20];
    s32 unk44;
    s32 unk48;
    u8 pad4c[0x18];
    s32 unk64;
    s32 unk68;
    s32 unk6c;
    s32 unk70;
    s32 unk74;
    u8 pad78[4];
    s32 unk7c;
    s32 unk80;
    u8 pad84[0xc];
    s32 unk90;
    s32 unk94;
    s32 unk98;
    s32 unk9c;
    s32 unka0;
    UnkMap_022e8090 *map;
} UnkWork_022e8090;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e8090 *work;
} UnkTask_022e8090;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1c;
} UnkCfg_022e8090;

extern UnkCfg_022e8090 gGameState;

UnkTask_022e8090 *Task_Create(void (*func)(void), s32 a, s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void ZoneMap_State_Unk_ov001_022e7428(void);
void ZoneMap_State_WaitTimer(void);
s32 ZoneMap_IsBlaze(void);
s32 ZoneMap_GetSonicStoryStep(void);
s32 ZoneMap_GetBlazeStoryStep(void);
s32 ZoneMap_Unk_ov001_022e26d0(void);
s32 ZoneMap_GetNodeForCurrentZone(void);
s32 ZoneMap_GetNodeForStoryStep(void);
void ZoneMap_RevealNodesForStory(UnkWork_022e8090 *w);
void ZoneMap_SetCharModelRotY(void *p, s32 a);
void ZoneMap_CalcClampedScroll(void *p, s32 x, s32 y, s32 *a, s32 *b);
void ZoneMap_SetMapBgIndex(void *p, s32 a);
BOOL ZoneMap_IsSonicNaviAvailable(s32 a);
BOOL ZoneMap_Unk_ov001_022e2e9c(s32 a);
BOOL ZoneMap_Unk_ov001_022e2df0(s32 a);
BOOL ZoneMap_IsBlazeNaviAvailable(s32 a);
BOOL ZoneMap_Unk_ov001_022e2e40(s32 a);
BOOL ZoneMap_Unk_ov001_022e2dcc(s32 a);
void ZoneMap_SetNaviMessage(s32 a, s32 b);
void ZoneMap_CreateFadeTask(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ZoneMap_CheckNewEmerald(UnkWork_022e8090 *w);

#pragma thumb on
UnkWork_022e8090 *ZoneMap_Unk_ov001_022e8090(UnkMap_022e8090 *map, s32 alt, s32 mode) {
    UnkTask_022e8090 *t;
    UnkWork_022e8090 *w;
    vu32 z;

    if (mode != 0) {
        t = Task_Create(ZoneMap_State_Unk_ov001_022e7428, 0, 0, 0x2000, 0x1000, 0xa8);
    } else {
        t = Task_Create(ZoneMap_State_WaitTimer, 0, 0, 0x2000, 0x1000, 0xa8);
    }
    w = t->work;
    z = 0;
    MIi_CpuClear32(z, w, 0xa8);
    w->unk0 = ZoneMap_IsBlaze();
    if (w->unk0 == 0) {
        w->unk4 = ZoneMap_GetSonicStoryStep();
        w->unk8 = 0;
    } else {
        w->unk4 = 0;
        w->unk8 = ZoneMap_GetBlazeStoryStep();
    }
    w->unkc = ZoneMap_Unk_ov001_022e26d0();
    if (mode != 0 || (alt != 0 && gGameState.unk1c == 7)) {
        w->unk14 = ZoneMap_GetNodeForCurrentZone();
    } else {
        w->unk14 = ZoneMap_GetNodeForStoryStep();
    }
    w->unk18 = 0x12;
    w->unk1c = 0x17;
    w->unk20 = 0;
    w->unk44 = 0x17;
    w->unk48 = 0x12;
    w->unk74 = 0;
    w->unk7c = 0;
    w->unk80 = 8;
    w->unk90 = 0;
    w->unk94 = 0x24;
    w->unk98 = 0x23;
    w->unk9c = 0;
    w->unka0 = 0;
    w->map = map;
    w->flags = 0;
    if (mode != 0) {
        w->flags |= 1;
        w->flags |= 4;
    }
    ZoneMap_RevealNodesForStory(w);
    w->unk64 = map->nodes[w->unk14].x << 12;
    w->unk68 = map->nodes[w->unk14].y << 12;
    ZoneMap_SetCharModelRotY(map->obj1994, 0x1000);
    ZoneMap_CalcClampedScroll(map->sub, w->unk64 >> 12, w->unk68 >> 12, &w->unk6c, &w->unk70);
    ZoneMap_SetMapBgIndex(map->sub, w->unkc);
    switch (w->unk0) {
    case 0:
        if (ZoneMap_IsSonicNaviAvailable(w->unk4)) {
            if (alt != 0) {
                if (ZoneMap_Unk_ov001_022e2e9c(w->unk4)) {
                    ZoneMap_SetNaviMessage(map->unk1cc4, 0x1b);
                } else {
                    ZoneMap_SetNaviMessage(map->unk1cc4, 3);
                }
            } else {
                if (ZoneMap_Unk_ov001_022e2df0(w->unk4)) {
                    ZoneMap_SetNaviMessage(map->unk1cc4, 3);
                } else {
                    ZoneMap_SetNaviMessage(map->unk1cc4, 0x1a);
                }
            }
        } else {
            ZoneMap_SetNaviMessage(map->unk1cc4, 0);
        }
        break;
    case 1:
        if (ZoneMap_IsBlazeNaviAvailable(w->unk8)) {
            if (alt != 0) {
                if (ZoneMap_Unk_ov001_022e2e40(w->unk8)) {
                    ZoneMap_SetNaviMessage(map->unk1cc4, 0x1b);
                } else {
                    ZoneMap_SetNaviMessage(map->unk1cc4, 3);
                }
            } else {
                if (ZoneMap_Unk_ov001_022e2dcc(w->unk8)) {
                    ZoneMap_SetNaviMessage(map->unk1cc4, 3);
                } else {
                    ZoneMap_SetNaviMessage(map->unk1cc4, 0x1a);
                }
            }
        } else {
            ZoneMap_SetNaviMessage(map->unk1cc4, 0);
        }
        break;
    }
    if (mode == 0) {
        ZoneMap_CreateFadeTask(0x10, 0, 8, 0, 1, 1);
    } else {
        w->unk74 = 0x10;
    }
    ZoneMap_CheckNewEmerald(w);
    if (mode != 0) {
        if (w->unk9c != 0) {
            w->flags |= 8;
        }
        if (w->unka0 != 0) {
            w->flags |= 0x10;
        }
        w->unk9c = 0;
        w->unka0 = 0;
    }
    if (w->unk0 == 0 && (w->unk4 == 0x16 || w->unk4 == 0x1b)) {
        w->unk90 = 1;
    }
    return w;
}
#pragma thumb reset
