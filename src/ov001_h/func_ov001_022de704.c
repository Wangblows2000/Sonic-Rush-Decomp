#include <nitro/types.h>

typedef struct UnkStruct_022de704_w {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[0x5c - 8];
    u8 objs[2][0x138];
} UnkStruct_022de704_w;

void func_020316f4(void *a);
void func_020315e0(void *a);

void func_ov001_022de704(u8 *s, UnkStruct_022de704_w *w) {
    u8 *p;
    u8 i;
    for (i = 0; i < 2; i++) {
        p = w->objs[i];
        func_020316f4(p);
        func_020315e0(p);
    }
    p = s + 0xa84;
    *(u32 *)(p + 0x4c) = w->unk4;
    func_020316f4(p);
    func_020315e0(p);
}
