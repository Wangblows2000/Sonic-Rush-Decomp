/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SaveData_VerifyChecksum();
s32 SaveData_CalcChecksum();
s32 memcmp();

s32 SaveData_Compare(s32 p0, s32 p1) {
    s32 t0 = SaveData_VerifyChecksum();
    if (t0 == 0) {
        return 0;
    }
    s32 t1 = SaveData_VerifyChecksum(p1);
    if (t1 == 0) {
        return 0;
    }
    s32 t2 = SaveData_CalcChecksum(p0);
    s32 t3 = SaveData_CalcChecksum(p1);
    if (t2 != t3) {
        return 0;
    }
    s32 t4 = memcmp(p0, p1, 0x314);
    return (t4 == 0);
}
