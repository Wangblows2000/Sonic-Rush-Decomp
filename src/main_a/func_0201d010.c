#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; } UnkStruct_0201d010;
extern UnkStruct_0201d010 data_02083e10;
void func_0201c9d0(void);

void func_0201d010(void) {
    if (data_02083e10.unk0 == 0) return;
    if (data_02083e10.unk4 != 1) return;
    func_0201c9d0();
}
