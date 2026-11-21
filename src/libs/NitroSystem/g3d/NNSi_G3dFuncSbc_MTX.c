#include <nitro/types.h>
#include <NitroSystem/g3drs.h>

void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);

void NNSi_G3dFuncSbc_MTX(UnkRS *rs) {
    if (!(rs->flag & 0x200) && (rs->flag & 1)) {
        int t;
        BOOL skip = UnkRS_CbA(rs, 3, &t);
        if (!skip) {
            u32 arg = rs->c[1];
            if (!(rs->flag & 0x100)) NNS_G3dGeBufferOP_N(0x14, &arg, 1);
        }
        UnkRS_CbC(rs, 3, t);
    }
    rs->c += 2;
}
