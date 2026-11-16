#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 unk4[0xf8];
    u32 flags;
} UnkStruct_02084d38;

extern UnkStruct_02084d38 data_02084d38;
extern u8 data_02084df4[];
void func_02025268(s32 a, void *b, s32 c);

void func_02038228(void) {
    UnkStruct_02084d38 *d = (UnkStruct_02084d38 *)((u32)&data_02084d38 | 0);
    func_02025268(d->unk0, d->unk4, 0x1e);
    func_02025268(0x1b19, data_02084df4, 0xf);
    data_02084d38.flags &= ~1;
    data_02084d38.flags &= ~2;
}
