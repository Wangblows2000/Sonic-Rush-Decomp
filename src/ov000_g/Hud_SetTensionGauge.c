#include <nitro/types.h>

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x270 - 4];
    s16 unk270;
    s16 unk272;
    u16 unk274;
    u16 unk276;
    s32 unk278[5];
    u16 unk28C;
    u16 unk28E;
    u16 unk290;
    s16 unk292;
    s16 unk294;
    u8 pad296[2];
    s32 unk298;
} UnkStruct_02336050_S;

typedef struct {
    u8 pad0[0x688];
    UnkStruct_02336050_S unk688;
} UnkStruct_02336050;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_02336050 *work;
} UnkStruct_02336050_Task;

extern UnkStruct_02336050_Task *gHudTask;

s32 Hud_TensionToGaugeValue(s16 d);

void Hud_SetTensionGauge(s32 v, s32 mode) {
    UnkStruct_02336050_S *s;
    u32 i;

    if ((u32)v > 300)
        v = 300;
    s = &gHudTask->work->unk688;
    s->unk298 = mode;
    if (s->unk270 < s->unk272) {
        if (s->unk270 >= v) {
            if (s->unk298 == 1)
                s->unk290 = 0x28;
            s->unk270 = v;
            s->unk278[4] = 0;
            return;
        }
        s->unk272 = s->unk270;
        s->unk270 = v;
        s->unk274 = s->unk276 = 0;
        s->unk278[4] = Hud_TensionToGaugeValue(s->unk270 - s->unk272);
        return;
    }
    if (s->unk270 > s->unk272) {
        if (s->unk270 <= v) {
            s->unk270 = v;
            v = Hud_TensionToGaugeValue(s->unk270 - s->unk272);
            if (s->unk278[4] < v)
                s->unk278[4] = v;
            return;
        }
        if (s->unk298 == 1) {
            s->unk290 = 0x28;
            s->unk272 = s->unk270;
            s->unk270 = v;
            s->unk274 = s->unk276 = 0;
            s->unk278[4] = 0;
            for (i = 0; i < 4; i++)
                s->unk278[i] = 0x1000;
            return;
        }
        s->unk270 = v;
        s->unk274 = s->unk276 = 0;
        return;
    }
    if (s->unk270 > v) {
        if (s->unk298 == 1)
            s->unk290 = 0x28;
        s->unk278[4] = 0;
        for (i = 0; i < 4; i++)
            s->unk278[i] = 0x1000;
    } else if (s->unk270 < v) {
        s->unk278[4] = Hud_TensionToGaugeValue(v - s->unk270);
    } else {
        s->unk278[4] = 0;
    }
    s->unk270 = v;
    s->unk274 = s->unk276 = 0;
}
