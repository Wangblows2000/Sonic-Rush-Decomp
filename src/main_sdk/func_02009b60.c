#include <nitro/types.h>

void func_02008ee8(void);
void func_02008ed4(void);
typedef struct { u8 pad[0x3c]; u32 offs[1]; } UnkStruct_02009b60;
void *func_02009b60(UnkStruct_02009b60 *p, s32 i) {
    u8 *r;
    u32 off;
    func_02008ee8();
    off = p->offs[i];
    if (off != 0) { if (off < 0x2000000) r = (u8 *)p + off; else r = (u8 *)off; } else r = 0;
    func_02008ed4();
    return r;
}
