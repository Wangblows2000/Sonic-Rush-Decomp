#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 flags;
} UnkStruct_022c4560;

typedef struct {
    u8 *buf0;
    u32 size0;
    u8 *buf1;
    u32 size1;
} UnkStruct_022c4520;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4520 data_022c4520;

void *StageEvent_GetMapObjectList(void);
void *StageEvent_GetRingList(void);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);

static inline void MI_CpuCopy32(const void *src, void *dest, u32 size) {
    MIi_CpuCopy32(src, dest, size);
}

static inline void MI_CpuCopy16(const void *src, void *dest, u32 size) {
    MIi_CpuCopy16(src, dest, size);
}

void Stage_RestoreEventState(void) {
    UnkStruct_022c4520 *s;

    if (!(gGameState.flags & 0x40))
        return;
    s = &data_022c4520;
    if ((s->size0 & 3) == 0)
        MI_CpuCopy32(s->buf0, StageEvent_GetMapObjectList(), s->size0);
    else
        MI_CpuCopy16(s->buf0, StageEvent_GetMapObjectList(), s->size0);
    if ((s->size1 & 3) == 0)
        MI_CpuCopy32(s->buf1, StageEvent_GetRingList(), s->size1);
    else
        MI_CpuCopy16(s->buf1, StageEvent_GetRingList(), s->size1);
}
