#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
} UnkStruct_Ev;

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
    u16 unk4;
} UnkStruct_Ent;

typedef struct {
    u8 pad[0x326];
    u8 unk326;
} UnkStruct_Player;

typedef struct {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_Player *gPlayers;

void NetPacket_ResetRead(void);
UnkStruct_Ev *NetPacket_FindNextData(s32 a, s32 b);
u8 *StageEvent_GetMapObjectList(void);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Player_ApplySlowdown(UnkStruct_Player *p);
void Player_ApplyConfusion(UnkStruct_Player *p);
void Player_DrainTension(UnkStruct_Player *p);
void Player_WarpToRival(UnkStruct_Player *p);

void Stage_NetProcessRivalEvents(void) {
    UnkStruct_Ev *o;
    u16 i;
    UnkStruct_Ent *e;
    u8 *base;
    u16 off;

    if (!(gGameState.unk18 & 0x20))
        return;
    NetPacket_ResetRead();
    for (i = 0; i < 2; i++) {
        if (i == gPlayers->unk326)
            continue;
        for (;;) {
            o = NetPacket_FindNextData(2, i);
            if (o == NULL)
                break;
            base = StageEvent_GetMapObjectList();
            off = o->unk2;
            e = (UnkStruct_Ent *)((u32)off + (u32)base);
            e->unk4 |= 0x2000;
            if (*(u8 *)((u32)off + (u32)base) == 0xff && e->unk2 != 0x30 && e->unk2 != 0x31)
                continue;
            switch (o->unk0) {
            case 7:
                Sound_PlaySeqArcEx(0, 0, 0xdf);
                Player_ApplySlowdown(gPlayers);
                break;
            case 8:
                Sound_PlaySeqArcEx(0, 0, 0xdf);
                Player_ApplyConfusion(gPlayers);
                break;
            case 9:
                Sound_PlaySeqArcEx(0, 0, 0xdf);
                Player_DrainTension(gPlayers);
                break;
            case 10:
                Sound_PlaySeqArcEx(0, 0, 0xdf);
                Player_WarpToRival(gPlayers);
                break;
            case 2:
                e->unk0 = 0xff;
                break;
            }
        }
    }
}
