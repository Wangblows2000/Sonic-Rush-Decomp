#include <nitro/types.h>

typedef struct { u32 m[9]; } UnkMtx_02030850;
typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_02030850;
extern u8 data_02084d84[];
extern UnkMtx_02030850 data_02084df4;
extern UnkStruct_02030850 data_02084d38;
void func_02038328(void *src, UnkMtx_02030850 *dst);
void MI_Copy36B(const void *src, void *dst);

void func_02030850(void) {
    UnkMtx_02030850 m;
    func_02038328(data_02084d84, &m);
    MI_Copy36B(&m, &data_02084df4);
    data_02084d38.unkFC &= ~0xA4;
}
