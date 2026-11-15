#include <nitro/types.h>
#include <main_a2/g3drs.h>

void func_02025268(u32 op, const void *args, u32 n);

void func_02024a40(UnkRS *rs) {
    if (!(rs->flag & 0x200) && (rs->flag & 1)) {
        int t;
        BOOL skip = UnkRS_CbA(rs, 3, &t);
        if (!skip) {
            u32 arg = rs->c[1];
            if (!(rs->flag & 0x100)) func_02025268(0x14, &arg, 1);
        }
        UnkRS_CbC(rs, 3, t);
    }
    rs->c += 2;
}
