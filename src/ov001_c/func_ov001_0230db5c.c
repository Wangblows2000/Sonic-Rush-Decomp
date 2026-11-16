#include <nitro/types.h>

typedef struct UnkStruct_0230db5c {
    u8 pad0[0x3c];
    u8 objs[7][0x138];
    u8 obj8C4[0x9fc - 0x8c4];
    void *res[4];
    void *unkA0C;
    u8 padA10[0xa40 - 0xa10];
    void *unkA40;
    void *unkA44;
} UnkStruct_0230db5c;

extern UnkStruct_0230db5c *data_ov001_0231f654;
extern void *gMainHeap;

void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void Model_Release(void *o);
void func_0202566c(void *res);

static inline void Free_0230db5c(void *p) {
    u32 size;
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void func_ov001_0230db5c(void) {
    s32 i;

    Free_0230db5c(data_ov001_0231f654->unkA44);
    Free_0230db5c(data_ov001_0231f654->unkA40);
    data_ov001_0231f654->unkA44 = 0;
    data_ov001_0231f654->unkA40 = 0;
    for (i = 0; i < 7; i++)
        Model_Release(data_ov001_0231f654->objs[i]);
    Model_Release(data_ov001_0231f654->obj8C4);
    for (i = 0; i < 4; i++) {
        func_0202566c(data_ov001_0231f654->res[i]);
        Free_0230db5c(data_ov001_0231f654->res[i]);
        data_ov001_0231f654->res[i] = 0;
    }
    Free_0230db5c(data_ov001_0231f654->unkA0C);
    data_ov001_0231f654->unkA0C = 0;
    Free_0230db5c(data_ov001_0231f654);
    data_ov001_0231f654 = 0;
}
