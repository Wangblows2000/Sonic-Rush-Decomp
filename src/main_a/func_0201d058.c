#include <nitro/types.h>

void func_0201c9d0(void);
void func_020205a0(void *f, s32 target, s32 frame);

typedef struct { u32 unk0; u32 unk4; u8 unk8[0x34]; u8 fader[0x10]; u32 unk4C; } UnkStruct_0201d058;
extern UnkStruct_0201d058 data_02083e10;

void func_0201d058(s32 frames) {
    UnkStruct_0201d058 *p = &data_02083e10;
    if (p->unk0 == 0) return;
    if (p->unk4 != 0) return;
    if (frames == 0) {
        func_0201c9d0();
        return;
    }
    func_020205a0(p->fader, 0, frames);
    p->unk4C = 1;
}
