#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x328 - 0x20C];
    s16 unk328;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    s32 unk434;
    u8 pad438[0x57C - 0x438];
    u8 unk57C;
} UnkStruct_022f12c0;

extern UnkStruct_Player *data_ov000_0236ac60;

void func_ov000_022f490c(UnkStruct_022f12c0 *s);
void func_ov000_022f898c(void);
void func_ov000_022f3f8c(void);
void func_ov000_022f88dc(void);

void func_ov000_022f12c0(UnkStruct_022f12c0 *s) {
    if ((u16)(s16)(data_ov000_0236ac60->unk328 - 0xf) <= 1)
        s->unk434 = 1;
    func_ov000_022f490c(s);
    if (!(s->unk57C & 2))
        return;
    if (s->unk434 != 0) {
        func_ov000_022f898c();
        return;
    }
    func_ov000_022f3f8c();
    func_ov000_022f88dc();
}
