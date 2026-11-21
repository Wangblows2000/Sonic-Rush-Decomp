#include <nitro/types.h>
#include <main_a2/g3drs.h>

void NNS_G3dGeSendDL(const void *p, u32 size);

void NNSi_G3dFuncSbc_CALLDL(UnkRS *rs) {
    int t;
    BOOL skip = UnkRS_CbA(rs, 10, &t);
    if (!(rs->flag & 0x100) && !skip) {
        u8 *c = rs->c;
        u32 ofs = c[1] | (c[2] << 8) | (c[3] << 16) | (c[4] << 24);
        u32 sz = c[5] | (c[6] << 8) | (c[7] << 16) | (c[8] << 24);
        NNS_G3dGeSendDL(c + ofs, sz);
    }
    UnkRS_CbC(rs, 10, t);
    rs->c += 9;
}
