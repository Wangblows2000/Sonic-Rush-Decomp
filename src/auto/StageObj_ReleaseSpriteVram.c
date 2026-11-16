/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjVramRef_Release();
s32 ObjVram_Free();

void StageObj_ReleaseSpriteVram(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x90);
    if (t0 != 0) {
        ObjVramRef_Release(t0, 0);
        ObjVramRef_Release((*(u32 *)((u8 *)p0 + 0x90) + 0x8), 1);
        return;
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0xc8);
    if (t1 != 0) {
        ObjVram_Free(0, t1);
    }
    *(u32 *)((u8 *)p0 + 0xc8) = 0;
    s32 t2 = *(u32 *)((u8 *)p0 + 0x110);
    if (t2 != 0) {
        ObjVram_Free(1, t2);
    }
    *(u32 *)((u8 *)p0 + 0x110) = 0;
}
