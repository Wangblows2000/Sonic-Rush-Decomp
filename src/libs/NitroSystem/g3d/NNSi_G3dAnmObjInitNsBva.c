#include <nitro/types.h>

extern u32 data_02077ac8;

typedef struct { u8 unk0[8]; u32 unk8; u32 unkC; u8 unk10[9]; u8 count; u16 unk1A[1]; } UnkStruct_020284a0;
typedef struct { u8 unk0[0x17]; u8 count; } UnkRes_020284a0;

void NNSi_G3dAnmObjInitNsBva(UnkStruct_020284a0 *s, u32 a, UnkRes_020284a0 *r) {
    u32 i;
    s->unkC = data_02077ac8;
    s->count = r->count;
    s->unk8 = a;
    for (i = 0; i < s->count; i++) {
        s->unk1A[i] = i | 0x100;
    }
}
