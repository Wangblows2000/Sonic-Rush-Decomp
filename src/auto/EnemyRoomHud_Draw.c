/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gTaskCurrent[];
s32 StageObj_Draw2D();
s32 StageObj_Draw2DEx();
extern u8 gPlayers[];

void EnemyRoomHud_Draw(void) {
    u32 t0 = *(u32 *)gTaskCurrent;
    u32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    StageObj_Draw2D(t1, (t1 + 0x94), 0);
    u32 t2 = *(u32 *)((u8 *)t1 + 0x54);
    s16 t3 = *(s16 *)((u8 *)t1 + 0x44);
    s16 t4 = *(s16 *)((u8 *)t1 + 0x46);
    if ((t2 & 0x1000) != 0) {
        s16 t5 = *(s16 *)((u8 *)(t1 + 0x200) + 0x54);
        *(u16 *)((u8 *)t1 + 0x44) = t5;
        s16 t6 = *(s16 *)((u8 *)(t1 + 0x200) + 0x58);
        *(u16 *)((u8 *)t1 + 0x46) = t6;
        u32 t7 = *(u32 *)((u8 *)t1 + 0x10);
        u32 t8 = *(u32 *)((u8 *)t1 + 0x14);
        s16 t9 = *(s16 *)((u8 *)(t1 + 0x200) + 0x5c);
        s16 t10 = *(s16 *)((u8 *)(t1 + 0x200) + 0x60);
        StageObj_Draw2DEx(((t7 - 0xb00) + (t9 << 8)), ((t8 + 0xe00) + (t10 << 8)), 0, t1, (t1 + 0x134), 0, 0);
    }
    u32 t11 = *(u32 *)((u8 *)t1 + 0x54);
    if ((t11 & 0x2000) != 0) {
        s16 t12 = *(s16 *)((u8 *)(t1 + 0x200) + 0x56);
        *(u16 *)((u8 *)t1 + 0x44) = t12;
        s16 t13 = *(s16 *)((u8 *)(t1 + 0x200) + 0x5a);
        *(u16 *)((u8 *)t1 + 0x46) = t13;
        u32 t14 = *(u32 *)((u8 *)t1 + 0x10);
        u32 t15 = *(u32 *)((u8 *)t1 + 0x14);
        s16 t16 = *(s16 *)((u8 *)(t1 + 0x200) + 0x5e);
        s16 t17 = *(s16 *)((u8 *)(t1 + 0x200) + 0x62);
        StageObj_Draw2DEx(((t14 + 0xb00) + (t16 << 8)), ((t15 + 0xe00) + (t17 << 8)), 0, t1, (t1 + 0x1c4), 0, 0);
    }
    *(u16 *)((u8 *)t1 + 0x44) = t3;
    *(u16 *)((u8 *)t1 + 0x46) = t4;
    u32 t18 = *(u32 *)gPlayers;
    u32 t19 = *(u32 *)((u8 *)t18 + 0x32c);
    if ((t19 & 0x400) != 0) {
        u32 t20 = *(u32 *)((u8 *)t1 + 0x54);
        *(u32 *)((u8 *)t1 + 0x54) = (t20 | 4);
    }
}
