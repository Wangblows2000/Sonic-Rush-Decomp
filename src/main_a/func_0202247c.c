#include <nitro/types.h>

typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_0202247c;
extern UnkStruct_0202247c data_02084d38;
void func_02025268(u32 a, void *b, u32 c);

void func_0202247c(void) {
    u32 *p = (u32 *)&data_02084d38;
    func_02025268(*p++, p, 0x3E);
    data_02084d38.unkFC &= ~1;
    data_02084d38.unkFC &= ~2;
}
