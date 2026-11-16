#include <nitro/types.h>

typedef struct { u32 unk0; s32 unk4; } UnkEntry_0201c4e4;
extern UnkEntry_0201c4e4 data_02083d74[];
extern u8 data_02083d28[];

typedef struct {
    u8 unk0[0x20];
    s32 unk20;
    s32 unk24_0 : 1;
    u32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 unk44[4];
    s32 count;
    u8 ids[1];
} UnkStruct_0201c4e4;

void NNS_SndStrmStop(UnkStruct_0201c4e4 *s);
u32 _u32_div_f(u32 a, u32 b);
s32 NNS_SndAllocAlarm(void);
void SND_SetupChannelPcm(s32 ch, s32 format, u32 data, s32 loop, s32 loopStart, s32 loopLen, s32 volume, s32 shift, s32 timer, s32 pan);
void SND_SetupAlarm(s32 alarm, u32 tick, u32 period, void (*cb)(void *), void *arg);
void func_0201c2dc(void *p);
void NNS_FndAppendListObject(void *list, void *obj);
s32 OS_DisableInterrupts(void);
void OS_RestoreInterrupts(s32 e);
void StrmCallback(void *p, u32 v);

BOOL NNS_SndStrmSetup(UnkStruct_0201c4e4 *s, s32 format, u32 data, u32 rate, s32 timer, s32 arg5, s32 arg6, s32 arg7) {
    u32 period;
    u32 size;
    s32 i;
    s32 en;
    u8 id;

    if (s->unk24_0) NNS_SndStrmStop(s);
    size = _u32_div_f(rate, s->count * (arg5 << 5));
    s->unk28 = (size * arg5) << 5;
    size = s->unk28;
    if (format == 1) {
        size >>= 1;
    }
    period = _u32_div_f(timer * size, arg5);
    s->unk40 = NNS_SndAllocAlarm();
    if (s->unk40 < 0) return FALSE;
    for (i = 0; i < s->count; i++) {
        id = s->ids[i];
        data_02083d74[id].unk0 = data + s->unk28 * i;
        data_02083d74[id].unk4 = 0;
        SND_SetupChannelPcm(id, format, data_02083d74[id].unk0, 1, 0, s->unk28 >> 2, 0x7F, 0, timer << 5, 0x40);
    }
    SND_SetupAlarm(s->unk40, period, period, func_0201c2dc, s);
    NNS_FndAppendListObject(data_02083d28, s);
    s->unk20 = format;
    s->unk2C = arg5;
    s->unk30 = arg6;
    s->unk34 = arg7;
    s->unk38 = 0;
    s->unk3C = 0;
    s->unk24_0 = 1;
    en = OS_DisableInterrupts();
    s->unk2C = 1;
    StrmCallback(s, 0);
    s->unk2C = arg5;
    OS_RestoreInterrupts(en);
    return TRUE;
}
