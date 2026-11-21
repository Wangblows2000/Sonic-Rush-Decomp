#include <nitro/types.h>

typedef struct { u16 a, b, c, d; } UnkStruct_0200d5f8;
typedef struct { u8 pad[4]; UnkStruct_0200d5f8 pos; u8 pad2[0x28]; u16 x34; } UnkStruct_0207f02c;
extern UnkStruct_0207f02c data_0207f02c;
void TP_WaitBusy(u32 m);
u32 TP_WaitRawResult(UnkStruct_0200d5f8 *out) {
    TP_WaitBusy(1);
    if (data_0207f02c.x34 & 1) return 1;
    *out = data_0207f02c.pos;
    return 0;
}
