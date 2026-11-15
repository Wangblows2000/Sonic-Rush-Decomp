#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct { u8 unk0[8]; u32 ofs; u32 size; } UnkData_02024478;

void func_02025364(const void *p, u32 size);

void func_02024478(UnkRS *rs, u32 unused, UnkData_02024478 *d) {
    int t;
    BOOL skip = UnkRS_CbA(rs, 5, &t);
    if (!skip && !(rs->flag & 0x100)) {
        func_02025364((u8 *)d + d->ofs, d->size);
    }
    UnkRS_CbB(rs, 5, &t);
    UnkRS_CbC(rs, 5, t);
}
