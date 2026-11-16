#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x20];
    u16 unk20;
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x328 - 0x20C];
    s16 unk328;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s32 unk434;
    u8 pad438[0x43C - 0x438];
    s32 unk43C;
    u16 unk440;
    u16 unk442;
    u8 pad444[0x458 - 0x444];
    s32 unk458;
} UnkStruct_022f1374;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_0208922c data_0208922c;
extern UnkStruct_Player *data_ov000_0236ac60;
extern u8 data_ov000_02365c5c[][3];

void func_ov000_022f490c(UnkStruct_022f1374 *w);
void func_ov000_022f168c(void);
void func_ov000_022f133c(void);

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

void func_ov000_022f1374(UnkStruct_022f1374 *w) {
    func_ov000_022f490c(w);
    if ((u16)(s16)(data_ov000_0236ac60->unk328 - 0xf) <= 1)
        w->unk434 = 1;
    if (w->unk442-- != 0)
        return;
    data_0208922c.unk20 &= ~0xc0;
    w->unk440++;
    w->unk43C = Rand() & 1;
    if (w->unk440 < data_ov000_02365c5c[GetLevel()][w->unk458])
        w->base.unk68 = func_ov000_022f168c;
    else
        w->base.unk68 = func_ov000_022f133c;
}
