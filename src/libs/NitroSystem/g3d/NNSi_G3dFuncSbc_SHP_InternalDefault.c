#include <nitro/types.h>
#include <NitroSystem/g3drs.h>

typedef struct { u8 unk0[8]; u32 ofs; u32 size; } UnkData_02024478;

void NNS_G3dGeSendDL(const void *p, u32 size);

void NNSi_G3dFuncSbc_SHP_InternalDefault(UnkRS *rs, u32 unused, UnkData_02024478 *d) {
    int t;
    BOOL skip = UnkRS_CbA(rs, 5, &t);
    if (!skip && !(rs->flag & 0x100)) {
        NNS_G3dGeSendDL((u8 *)d + d->ofs, d->size);
    }
    UnkRS_CbB(rs, 5, &t);
    UnkRS_CbC(rs, 5, t);
}
