#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7c];
    s32 unk7c;
} UnkStruct_02084d38;

typedef struct {
    u32 unk0;
    u8 unk4[0x38];
} UnkStruct_02084db4;

extern UnkStruct_02084d38 data_02084d38;
extern UnkStruct_02084db4 data_02084db4;
extern u8 data_02084e30[];
void func_02025268(s32 a, void *b, s32 c);

void func_02038280(void) {
    s32 v;
    UnkStruct_02084db4 *d = (UnkStruct_02084db4 *)((u32)&data_02084db4 | 0);
    func_02025268(data_02084d38.unk7c, d->unk4, 0xe);
    func_02025268(0x2a, data_02084e30, 1);
    v = 2;
    func_02025268(0x10, &v, 1);
}
