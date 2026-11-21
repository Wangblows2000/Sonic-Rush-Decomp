#include <nitro/types.h>

typedef struct UnkStruct_02312138_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02312138_v;

typedef struct UnkStruct_02312138_t {
    u16 flags;
    u8 pad2[0x34 - 2];
} UnkStruct_02312138_t;

typedef struct UnkStruct_02312138 {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[4];
    s32 x;
    s32 y;
    s32 z;
    u8 pad2C[0x228 - 0x2c];
    s32 type;
} UnkStruct_02312138;

extern UnkStruct_02312138_t data_ov001_0231790a[];

s32 VEC_Mag(UnkStruct_02312138_v *v);

BOOL SpecialStagePlayer_IsGrounded(UnkStruct_02312138 *p) {
    if (data_ov001_0231790a[p->type].flags & 0x10) {
        UnkStruct_02312138_v v;
        if (p->z < -0x5000)
            return FALSE;
        v.x = p->x;
        v.y = p->y;
        v.z = 0;
        if (VEC_Mag(&v) >= 0x1e000 && p->y <= 0)
            return TRUE;
    } else {
        if (p->unk18 <= 0)
            return TRUE;
    }
    return FALSE;
}
