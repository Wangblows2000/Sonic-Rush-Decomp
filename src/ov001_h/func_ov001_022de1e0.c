#include <nitro/types.h>

typedef struct UnkStruct_022de1e0 {
    u8 pad0[0x10];
    s32 unk10;
} UnkStruct_022de1e0;

void func_020316f4(void *a);
void func_020315e0(void *a);

void func_ov001_022de1e0(UnkStruct_022de1e0 *s, u8 *w) {
    void *p;
    p = w + 0x18;
    func_020316f4(p);
    func_020315e0(p);
    p = w + 0x150;
    func_020316f4(p);
    func_020315e0(p);
    if (s->unk10 == 2) {
        p = w + 0x630;
        func_020316f4(p);
        func_020315e0(p);
    }
}
