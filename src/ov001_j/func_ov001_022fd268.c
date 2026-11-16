#include <nitro/types.h>

typedef struct UnkStruct_022fd268_t {
    void *res;
    void *anm[5];
} UnkStruct_022fd268_t;

typedef struct UnkStruct_022fd268 {
    u8 pad0[0x84];
    void *unk84;
    u8 pad88[0x1c8 - 0x88];
    s32 type;
} UnkStruct_022fd268;

extern void *data_ov001_0231f098[][6];

void *NNS_G3dGetTex(void *res);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, void *d);

void func_ov001_022fd268(UnkStruct_022fd268 *p, s32 frame) {
    s32 i;
    void *r;
    void *obj;
    void **t;

    s32 ty = p->type;
    obj = p->unk84;
    r = NNS_G3dGetTex(data_ov001_0231f098[ty][0]);
    for (i = 0; i < 5; i++) {
        if (data_ov001_0231f098[ty][i + 1] != NULL)
            Model_SetAnimation(obj, i, data_ov001_0231f098[ty][i + 1], frame, r);
    }
}
