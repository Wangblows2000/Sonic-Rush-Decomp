#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x42E - 0x20C];
    u16 unk42E;
    u8 pad430[0x440 - 0x430];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    u16 unk464;
    u8 pad466[0x470 - 0x466];
    s32 unk470;
} UnkStruct_022e8120;

extern UnkStruct_022c4560 gGameState;
extern u16 data_ov000_02363b84[][4];

void func_ov000_022e8538(UnkStruct_022e8120 *w);
void func_ov000_022e8050(void);
void func_ov000_022e7f00(void);
void func_ov000_022e7c9c(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void Boss7PartD5_MoveState_Unk_ov000_022e8120(UnkStruct_022e8120 *w) {
    u16 t;
    func_ov000_022e8538(w);
    if (w->unk470 > 0) {
        w->unk470--;
    }
    if (w->unk470 != 0) {
        return;
    }
    w->unk464--;
    if (w->unk464 != 0) {
        w->unk440 = func_ov000_022e8050;
        return;
    }
    t = data_ov000_02363b84[GetIdxB()][w->unk42E];
    if (t >= (Rand() & 0xf)) {
        w->unk440 = func_ov000_022e7f00;
    } else {
        w->unk440 = func_ov000_022e7c9c;
    }
}
