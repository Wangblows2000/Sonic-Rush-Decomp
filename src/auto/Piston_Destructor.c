/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201c070();
s32 ObjVramRef_Release();
s32 MapObject_Destructor();

void Piston_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_0201c070(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x1dc), 0);
    ObjVramRef_Release(*(u32 *)((u8 *)t0 + 0x29c), 0);
    ObjVramRef_Release((*(u32 *)((u8 *)t0 + 0x29c) + 0x8), 1);
    MapObject_Destructor(p0);
}
