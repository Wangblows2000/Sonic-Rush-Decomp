#include <nitro/types.h>

typedef struct {
    u16 c : 5;
    u16 pad5 : 11;
} UnkStruct_02089250;

typedef struct {
    u8 pad0[0x24];
    struct {
        u16 c : 5;
        u16 pad5 : 11;
    } unk24;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0x68];
    void *unk68;
} UnkStruct_022f0e38;

extern UnkStruct_02089250 data_02089250;
extern UnkStruct_0208922c gDisplayMain;

void func_ov000_022f0cb0(UnkStruct_022f0e38 *p);

void func_ov000_022f0e38(UnkStruct_022f0e38 *p) {
    data_02089250.c += 2;
    if (gDisplayMain.unk24.c >= 0x10)
        p->unk68 = func_ov000_022f0cb0;
}
