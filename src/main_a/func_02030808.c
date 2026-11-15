#include <nitro/types.h>

typedef struct { u32 m[9]; } UnkMtx_02030808;
typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_02030808;
extern u8 data_02084d84[];
extern UnkMtx_02030808 data_02084df4;
extern UnkStruct_02030808 data_02084d38;
void func_020382e0(void *src, UnkMtx_02030808 *dst);
void MI_Copy36B(const void *src, void *dst);

void func_02030808(void) {
    UnkMtx_02030808 m;
    func_020382e0(data_02084d84, &m);
    MI_Copy36B(&m, &data_02084df4);
    data_02084d38.unkFC &= ~0xA4;
}
