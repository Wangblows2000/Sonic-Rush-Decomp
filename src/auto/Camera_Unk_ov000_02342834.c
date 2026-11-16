/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
s32 NetPacket_ResetRead();
extern u8 gPlayers[];
s32 NetPacket_FindNextData();
extern u8 data_ov000_0236aadc[];
extern u8 data_ov000_0236aae0[];
s32 Camera_LockX();
s32 Camera_LockY();

void Camera_Unk_ov000_02342834(s32 p0) {
    if ((*(u32 *)((u8 *)gGameState + 0x18) & 0x20) == 0) {
        return;
    }
    if ((s32)p0 == 0) {
        return;
    }
    NetPacket_ResetRead((*(u32 *)((u8 *)gGameState + 0x18) & 0x20), gGameState);
    s32 t0 = NetPacket_FindNextData(1, *(u8 *)((u8 *)*(u32 *)((u8 *)gPlayers + ((s32)p0 << 2)) + 0x326));
    if (t0 == 0) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)gPlayers + ((s32)p0 << 2));
    *(u32 *)((u8 *)data_ov000_0236aadc + (*(u8 *)((u8 *)*(u32 *)((u8 *)gPlayers + ((s32)p0 << 2)) + 0x327) * 0x64)) = *(u32 *)((u8 *)t0 + 0x24);
    s32 t2 = (*(u8 *)((u8 *)t1 + 0x327) * 0x64);
    s32 t3 = *(u8 *)((u8 *)t1 + 0x327);
    *(u32 *)((u8 *)data_ov000_0236aae0 + t2) = *(u32 *)((u8 *)t0 + 0x28);
    Camera_LockX(p0, t2, data_ov000_0236aae0, t3);
    Camera_LockY(p0);
}
