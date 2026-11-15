#include <nitro/types.h>

typedef struct { u8 unk0[4]; u16 unk4; } UnkData_020278a8;
typedef struct { fx32 unk0; u32 unk4; UnkData_020278a8 *unk8; } UnkStruct_020278a8;
void func_02027290(UnkData_020278a8 *d, u32 b, fx32 c, void *a);

void func_020278a8(void *a, UnkStruct_020278a8 *s, u32 b) {
    fx32 max = s->unk8->unk4 << 12;
    fx32 f = s->unk0;
    if (f >= max) {
        f = max - 1;
    } else if (f < 0) {
        f = 0;
    }
    func_02027290(s->unk8, b, f, a);
}
