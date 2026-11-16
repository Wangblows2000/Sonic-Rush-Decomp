#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s16 unk48;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022c4590;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3AE - 0x20C];
    s16 unk3AE;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s16 unk434;
    u8 pad436[0x57C - 0x436];
    u8 unk57C;
} UnkStruct_022ef2a4;

extern UnkStruct_0208922c data_0208922c;
extern UnkStruct_022c4590 data_022c4590;
extern UnkStruct_Player *data_ov000_0236ac60;

void func_0204eb04(void);
void func_0204d5f8(void (*fn)(void));

void func_ov000_022ef2a4(UnkStruct_022ef2a4 *w) {
    w->unk434++;
    if (w->unk434 >= 0 && (w->unk434 & 3) == 0) {
        if (data_0208922c.unk48 > 0)
            data_0208922c.unk48--;
    }
    if (!(w->unk57C & 2))
        return;
    if (w->unk434 <= 0x12c)
        return;
    w->base.unk54 |= 4;
    data_022c4590.unk10 = data_ov000_0236ac60->unk3AE * 1000;
    func_0204d5f8(func_0204eb04);
}
