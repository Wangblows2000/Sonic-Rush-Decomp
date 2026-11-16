#include <nitro/types.h>

typedef struct UnkStruct_022c8404_e {
    u8 pad0[0x48];
} UnkStruct_022c8404_e;

typedef struct UnkStruct_022c8404_w {
    u8 pad0[0x558];
    u32 unk558;
} UnkStruct_022c8404_w;

typedef struct UnkStruct_022c8404_t {
    u8 pad0[0x10];
    UnkStruct_022c8404_e *unk10;
} UnkStruct_022c8404_t;

extern UnkStruct_022c8404_t *data_022b4574;
void func_02039bb0(s32 a, u32 b);
void func_ov001_022c7eb4(void *w);

void func_ov001_022c8404(void) {
    UnkStruct_022c8404_e *w;
    s32 i;
    w = data_022b4574->unk10;
    for (i = 0; i < 5; i++) {
        func_02039bb0(1, ((UnkStruct_022c8404_w *)&w[i])->unk558);
    }
    func_ov001_022c7eb4(w);
}
