#include <nitro/types.h>

typedef struct UnkStruct_022fdf9c {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022fdf9c;

void func_ov000_022fe7e8(UnkStruct_022fdf9c *s);
void func_02069090(UnkStruct_022fdf9c *s, void *p);

void func_ov000_022fdf9c(UnkStruct_022fdf9c *s) {
    if (s->unk5C & 8) {
        func_ov000_022fe7e8(s);
        return;
    }
    func_02069090(s, (u8 *)s + 0x20c);
}
