#include <ov000_d/mdl.h>

typedef struct {
    UnkStruct_MdlRender r;
    u8 pad98[0x10C - sizeof(UnkStruct_MdlRender)];
    u8 flag;
    u8 pad10D[0x138 - 0x10D];
    void *mdl;
    void *anm0;
    void *anm1;
    u8 pad144[4];
} UnkStruct_02360408_Slot;

typedef struct {
    u8 pad0[0xB68];
    UnkStruct_02360408_Slot slot[2];
    void *unkDF8;
} UnkStruct_02360408;

typedef struct {
    void *unk0;
    u8 pad4[4];
    void *unk8;
} UnkStruct_0236ac50;

extern void *gMainHeap;
extern char data_ov000_02369a88[];
extern u8 data_ov000_02369aa0[];
extern UnkStruct_0236ac50 data_ov000_0236ac50;
extern void *data_ov000_0236ac40;
extern void *data_ov000_0236ac48;
extern u8 data_ov000_02364ee8[];
extern u16 data_ov000_02364ee0[];

BOOL func_ov000_0235faf0(void);
void *File_Load(const char *path, void *dst);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u32 func_ov000_0235ffc4(void *res);
u32 func_ov000_0235ffb8(void *res);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void func_02021310(void *buf, void *a, void *b);
void *func_02021264(void *buf, s32 a);
void func_020257f8(void);
void func_020212e0(void *buf);
void Model_Init(UnkStruct_MdlRender *r, s32 a);
void Model_SetResource(UnkStruct_MdlRender *r, void *mdl, s32 a);
void func_02004084(void *p);
void func_02022270(s32 idx, u16 val);

static inline void *UnkAlloc_02360408(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = Heap_Alloc(gMainHeap, size, align);
    if (func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x2467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void UnkFree_02360408(void *p) {
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

#pragma thumb on

void func_ov000_02360408(UnkStruct_02360408 *w) {
    vu32 zero;
    void *res;
    void *dst;
    u8 buf[0x5c];

    if (func_ov000_0235faf0()) {
        zero = 0;
        MIi_CpuClear32(zero, w->slot, sizeof(w->slot));
        w->slot[0].mdl = data_ov000_0236ac50.unk0;
        w->slot[0].anm0 = data_ov000_0236ac40;
        w->slot[0].anm1 = data_ov000_0236ac48;
        w->slot[1].mdl = data_ov000_0236ac50.unk8;
        w->slot[1].anm0 = data_ov000_0236ac40;
        w->slot[1].anm1 = data_ov000_0236ac48;
    } else {
        res = File_Load(data_ov000_02369a88, (void *)-1);
        w->unkDF8 = UnkAlloc_02360408(*(u32 *)res >> 8, 0x20);
        dst = w->unkDF8;
        switch (func_ov000_0235ffc4(res)) {
        case 0x10:
            func_02008648(res, dst);
            break;
        case 0x20:
            func_02008720(res, dst);
            break;
        case 0x30:
            func_020087f8(res, dst);
            break;
        default:
            Mem_Copy(Compress_GetData(res), dst, func_ov000_0235ffb8(res));
            break;
        }
        DC_StoreRange(dst, func_ov000_0235ffb8(res));
        UnkFree_02360408(res);
        func_02021310(buf, data_ov000_02369aa0, w->unkDF8);
        func_02021264(buf, 0);
        func_020257f8();
        w->slot[0].mdl = func_02021264(buf, 0);
        w->slot[0].anm0 = func_02021264(buf, 1);
        w->slot[0].anm1 = func_02021264(buf, 2);
        func_02021264(buf, 3);
        func_020257f8();
        w->slot[1].mdl = func_02021264(buf, 3);
        w->slot[1].anm0 = func_02021264(buf, 1);
        w->slot[1].anm1 = func_02021264(buf, 2);
        func_020212e0(buf);
    }
    Model_Init(&w->slot[0].r, 0);
    Model_SetResource(&w->slot[0].r, w->slot[0].mdl, 0);
    Model_SetAnimation(&w->slot[0].r, 0, w->slot[0].anm0, 0, NULL);
    Model_SetAnimation(&w->slot[0].r, 4, w->slot[0].anm1, 1, NULL);
    Model_Init(&w->slot[1].r, 0);
    Model_SetResource(&w->slot[1].r, w->slot[1].mdl, 0);
    Model_SetAnimation(&w->slot[1].r, 0, w->slot[1].anm0, 0x37, NULL);
    Model_SetAnimation(&w->slot[1].r, 4, w->slot[1].anm1, 1, NULL);
    w->slot[0].flag |= 4;
    w->slot[1].flag |= 4;
    func_02004084(data_ov000_02364ee8);
    func_02022270(0, data_ov000_02364ee0[0]);
    func_02022270(1, data_ov000_02364ee0[1]);
    func_02022270(2, data_ov000_02364ee0[2]);
}

#pragma thumb reset
