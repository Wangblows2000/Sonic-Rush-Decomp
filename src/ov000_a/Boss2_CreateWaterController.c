#include <nitro/types.h>

typedef struct UnkStruct_022cf8b0_t {
    void *unk0;
    void *unk4;
    s32 unk8;
} UnkStruct_022cf8b0_t;

typedef struct UnkTask_022cf8b0 {
    u8 pad[0x10];
    UnkStruct_022cf8b0_t *unk10;
} UnkTask_022cf8b0;

typedef struct UnkStruct_022cf8b0 {
    u8 pad[0x210];
    UnkTask_022cf8b0 *unk210;
} UnkStruct_022cf8b0;

UnkTask_022cf8b0 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void Boss2Water_Main(void);
void Boss2Water_Destructor(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

static inline void MI_CpuFill16(void *dest, u16 data, u32 size) {
    MIi_CpuClear16(*(volatile u16 *)&data, dest, size);
}

void Boss2_CreateWaterController(UnkStruct_022cf8b0 *s) {
    UnkTask_022cf8b0 *t = Task_Create(Boss2Water_Main, Boss2Water_Destructor, 0, 0x4ff, 2, 0x18);
    UnkStruct_022cf8b0_t *p = t->unk10;
    MI_CpuFill16(p, 0, 0x18);
    s->unk210 = t;
    p->unk0 = s;
    p->unk8 = -0x66000;
}
