#include <nitro/types.h>

typedef struct { s32 x, y, z; } UnkStruct_02306b18_v;
void func_ov001_023070f0(void *p);
void *SpecialStage_GetPlayer(void);
BOOL func_ov001_02310248(void *a, void *p);

void func_ov001_02306b18(u8 *p) {
    u8 *q = *(u8 **)(p + 0xd4);
    if (q != NULL) {
        if (*(u16 *)(q + 0x62) & 1) {
            *(UnkStruct_02306b18_v *)(p + 0x20) = *(UnkStruct_02306b18_v *)(q + 0x20);
            *(s32 *)(p + 0x28) += 0x1000;
        } else {
            func_ov001_023070f0(p);
        }
        return;
    }
    if (!func_ov001_02310248(SpecialStage_GetPlayer(), p)) {
        func_ov001_023070f0(p);
    }
}
