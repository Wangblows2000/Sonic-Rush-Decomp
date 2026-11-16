#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s32 unk434;
    u16 unk438;
    u8 pad43A[0x458 - 0x43A];
    s32 unk458;
} UnkStruct_022f324c;

extern UnkStruct_022c4560 gGameState;
extern s16 data_ov000_02363e54[][3];

void func_ov000_022f4ce4(UnkStruct_022f324c *s, s32 a, s32 b, s32 c);
void func_ov000_022f9188(void);
void func_ov000_022f3080(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void func_ov000_022f324c(UnkStruct_022f324c *s) {
    s->base.unk2E = 0;
    func_ov000_022f4ce4(s, 0x12, data_ov000_02363e54[GetIdxB()][s->unk458], 0);
    func_ov000_022f9188();
    s->base.unk68 = func_ov000_022f3080;
}
