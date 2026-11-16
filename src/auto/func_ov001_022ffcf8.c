/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_0230007c();
s32 ExtraBossObj_DestroyKind2();
extern u8 gExtraBossWork[];

void func_ov001_022ffcf8(void) {
    func_ov001_0230007c();
    ExtraBossObj_DestroyKind2();
    s32 t1 = (*(u32 *)*(u32 *)gExtraBossWork | 4);
    *(u32 *)*(u32 *)gExtraBossWork = t1;
}
