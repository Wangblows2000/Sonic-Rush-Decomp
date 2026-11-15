#include <nitro/types.h>

typedef struct UnkStruct_022fcce4 {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022fcce4;

void func_ov000_022fe7e8(UnkStruct_022fcce4 *s);

void func_ov000_022fcce4(UnkStruct_022fcce4 *s) {
    if (!(s->unk5C & 8))
        return;
    func_ov000_022fe7e8(s);
}
