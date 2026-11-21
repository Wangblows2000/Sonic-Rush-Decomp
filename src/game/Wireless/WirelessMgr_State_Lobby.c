#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u32 unk4;
    s32 state;
    u8 padc[0xb];
    u8 unk17;
    u16 unk18;
    u16 unk1a;
} UnkObj_0204211c;

typedef struct {
    u16 unk0;
    u16 unk2;
} UnkStruct_0224ffa0;

typedef struct {
    u8 pad[8];
    void (*func)(void);
} UnkTask_022b4574;

extern UnkStruct_0224ffa0 gWirelessSendBuffer;
extern UnkTask_022b4574 *gTaskCurrent;
u16 WH_GetBitmap(void);
u32 WirelessMgr_GetPlayerCount(void);
void WirelessMgr_State_ParentBeginRestart(void *);
void WirelessMgr_Main(void);

void WirelessMgr_State_Lobby(UnkObj_0204211c *o) {
    UnkStruct_0224ffa0 *s = (UnkStruct_0224ffa0 *)((u32)&gWirelessSendBuffer | 0);
    s->unk0 = WH_GetBitmap();
    if (WirelessMgr_GetPlayerCount() > 1) {
        o->state = 2;
        if (o->unk17 != 0) {
            s->unk2 |= 1;
            o->unk18 = 0;
            o->func = WirelessMgr_State_ParentBeginRestart;
            gTaskCurrent->func = WirelessMgr_Main;
        }
    } else {
        o->state = 1;
    }
    o->unk17 = 0;
    o->unk1a = 0;
}
