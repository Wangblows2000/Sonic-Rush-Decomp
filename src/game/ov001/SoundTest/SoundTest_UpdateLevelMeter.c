#include <nitro/types.h>

typedef struct {
    u16 x;
    u16 y;
} UnkPos_022c8e1c;

typedef struct {
    u8 pad0[0x10];
    s32 n;
    u8 pad14[0x28 - 0x14];
    s32 unk28;
    s32 unk2c;
    u8 pad30[0x1a8 - 0x30];
    s32 cnt;
    UnkPos_022c8e1c acc;
} UnkStruct_022c8e1c;

void SoundTest_GetChannelLevels(u16 *x, u16 *y);

#pragma thumb on
void SoundTest_UpdateLevelMeter(UnkStruct_022c8e1c *p) {
    UnkPos_022c8e1c pos;

    SoundTest_GetChannelLevels(&pos.x, &pos.y);
    if (p->n > 0 && p->n <= 0x7f) {
        pos.x = pos.x * 0x7f / p->n;
        pos.y = pos.y * 0x7f / p->n;
    } else {
        pos.x *= 0x7f;
        pos.y *= 0x7f;
    }
    if (pos.x > 0x200) pos.x = (pos.x - 0x200) / 3 + 0x200;
    if (pos.y > 0x200) pos.y = (pos.y - 0x200) / 3 + 0x200;
    if (pos.x > 0x300) pos.x = 0x300;
    if (pos.y > 0x300) pos.y = 0x300;
    p->acc.x += pos.x;
    p->acc.y += pos.y;
    p->cnt++;
    if (p->cnt >= 1) {
        pos.x = p->acc.x;
        pos.y = p->acc.y;
        p->cnt = 0;
        p->acc.x = 0;
        p->acc.y = 0;
        p->unk28 = (pos.x << 10) / 0x300 + 0x1000;
        p->unk2c = (pos.y << 10) / 0x300 + 0x1000;
    }
}
#pragma thumb reset
