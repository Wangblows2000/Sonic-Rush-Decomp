#include <ov000_d/mdl.h>

extern void *gMainHeap;
extern const char *data_ov000_02368e00;
extern const char *data_ov000_02368dfc;
extern const char *data_ov000_02368df8;
extern u8 data_ov000_023690d8[];
extern u8 data_ov000_023690dc[];
extern u8 data_ov000_023690e0[];
extern u8 data_ov000_02364eb8[];
extern u16 data_ov000_02364eb0[];

void *File_Load(const char *path, void *dst);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u32 func_ov000_02357804(void *res);
u32 func_ov000_023577f8(void *res);
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

static inline void *UnkAlloc_02357458(u32 size, s32 align) {
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

static inline void UnkFree_02357458(void *p) {
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

void func_ov000_02357458(UnkStruct_Mdl *w, BOOL flag) {
    vu32 zero;
    void *res;
    void *dst;
    u8 buf0[0x5c];
    u8 buf1[0x5c];
    u8 buf2[0x5c];

    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkStruct_Mdl));
    res = File_Load(data_ov000_02368e00, (void *)-1);
    w->unk2F8 = UnkAlloc_02357458(*(u32 *)res >> 8, 0x20);
    dst = w->unk2F8;
    switch (func_ov000_02357804(res)) {
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
        Mem_Copy(Compress_GetData(res), dst, func_ov000_023577f8(res));
        break;
    }
    DC_StoreRange(dst, func_ov000_023577f8(res));
    UnkFree_02357458(res);
    w->unk2FC = File_Load(data_ov000_02368dfc, (void *)-1);
    if (flag) {
        w->unk300 = File_Load(data_ov000_02368df8, (void *)-1);
    }
    func_02021310(buf0, data_ov000_023690d8, w->unk2F8);
    func_02021310(buf1, data_ov000_023690dc, w->unk2FC);
    if (flag) {
        func_02021310(buf2, data_ov000_023690e0, w->unk300);
    }
    func_02021264(buf0, 0);
    func_020257f8();
    w->slot[0].mdl14C = func_02021264(buf0, 0);
    w->slot[0].anm150 = func_02021264(buf1, 0);
    w->slot[0].anm154 = func_02021264(buf1, 1);
    if (flag) {
        w->slot[0].anm158 = func_02021264(buf2, 0);
        w->slot[0].anm15C = func_02021264(buf2, 1);
    } else {
        w->slot[0].anm158 = NULL;
        w->slot[0].anm15C = NULL;
    }
    func_02021264(buf0, 3);
    func_020257f8();
    w->slot[1].mdl14C = func_02021264(buf0, 3);
    w->slot[1].anm150 = func_02021264(buf1, 0);
    w->slot[1].anm154 = func_02021264(buf1, 1);
    if (flag) {
        w->slot[1].anm158 = func_02021264(buf2, 0);
        w->slot[1].anm15C = func_02021264(buf2, 1);
    } else {
        w->slot[1].anm158 = NULL;
        w->slot[1].anm15C = NULL;
    }
    if (flag) {
        func_020212e0(buf2);
    }
    func_020212e0(buf1);
    func_020212e0(buf0);
    Model_Init(&w->slot[0].r, 0);
    Model_SetResource(&w->slot[0].r, w->slot[0].mdl14C, 0);
    Model_SetAnimation(&w->slot[0].r, 0, w->slot[0].anm150, 0, NULL);
    Model_SetAnimation(&w->slot[0].r, 4, w->slot[0].anm154, 0, NULL);
    Model_Init(&w->slot[1].r, 0);
    Model_SetResource(&w->slot[1].r, w->slot[1].mdl14C, 0);
    Model_SetAnimation(&w->slot[1].r, 0, w->slot[1].anm150, 4, NULL);
    Model_SetAnimation(&w->slot[1].r, 4, w->slot[1].anm154, 4, NULL);
    w->x = 0;
    w->y = 0;
    w->z = 0;
    w->scale = 0x1a00;
    w->rx = 0;
    w->ry = 0;
    w->rz = 0;
    w->mode = 0;
    w->unk2C = 0;
    w->sel = 0;
    w->prevSel = 0;
    w->slot[0].cur = 0;
    w->slot[0].prev = -1;
    w->slot[0].flag = 0;
    w->slot[0].angOfs = 0xb1c7;
    w->slot[1].cur = 4;
    w->slot[1].prev = -1;
    w->slot[1].flag = 0;
    w->slot[1].angOfs = 0xae38;
    w->slot[0].unk120 |= 4;
    w->slot[1].unk120 |= 4;
    func_02004084(data_ov000_02364eb8);
    func_02022270(0, data_ov000_02364eb0[0]);
    func_02022270(1, data_ov000_02364eb0[1]);
    func_02022270(2, data_ov000_02364eb0[2]);
}

#pragma thumb reset
