#include <nitro/types.h>

void ExtraBossObj_PropagateParentDestroy(void);
void func_ov001_023057f4(void);
void func_ov001_02304a24(void);
void func_ov001_023041a8(void);
void func_ov001_022fea6c(void);
void ExtraBossObj_DestroyFlagged(void);

void func_ov001_022fef3c(void) {
    ExtraBossObj_PropagateParentDestroy();
    func_ov001_023057f4();
    func_ov001_02304a24();
    func_ov001_023041a8();
    func_ov001_022fea6c();
    ExtraBossObj_DestroyFlagged();
}
