#include <ov000_d/mdl2.h>

typedef struct {
    void *unk0;
    u8 pad4[4];
    void *unk8;
} UnkStruct_0236ac50;

extern void *gMainHeap;
extern char data_ov000_023692f8[];
extern char data_ov000_0236930c[];
extern u8 data_ov000_02369308[];
extern u8 data_ov000_02369324[];
extern u8 data_ov000_02369328[];
extern UnkStruct_0236ac50 data_ov000_0236ac50;
extern void *data_ov000_0236ac40;
extern void *data_ov000_0236ac48;
extern u8 data_ov000_02364ed0[];
extern u16 data_ov000_02364ec8[];

BOOL func_020513a8(void);
void *File_Load(const char *path, void *dst);
void *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
u32 func_ov000_0235d43c(void *res);
u32 func_ov000_0235d430(void *res);
void MI_UncompressLZ16(void *src, void *dst);
void MI_UncompressHuffman(void *src, void *dst);
void MI_UncompressRL16(void *src, void *dst);
void *Compress_GetData(void *res);
void Mem_Copy(void *src, void *dst, u32 size);
void func_02021310(void *buf, void *a, void *b);
void *func_02021264(void *buf, s32 a);
void NNS_G3dResDefaultSetup(void);
void func_020212e0(void *buf);
void Model_Init(UnkStruct_MdlRender *r, s32 a);
void Model_SetResource(UnkStruct_MdlRender *r, void *mdl, s32 a);
void G3X_SetEdgeColorTable(void *p);
void func_02022270(s32 idx, u16 val);

static inline void *UnkAlloc_0235dab4(u32 size, s32 align) {
    u8 *p;
    size = (size + 0x43) & ~0x1f;
    p = Heap_Alloc(gMainHeap, size, align);
    if (OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x2467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

static inline void UnkFree_0235dab4(void *p) {
    u32 size;
    if (OS_GetArenaLo(0) != p) {
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

void func_ov000_0235dab4(UnkStruct_Mdl2 *w) {
    void *res;
    void *dst;
    u8 buf0[0x5c];
    u8 buf1[0x5c];

    w->unk90C = File_Load(data_ov000_023692f8, (void *)-1);
    if (func_020513a8()) {
        func_02021310(buf1, data_ov000_02369308, w->unk90C);
        w->mdl8F0 = data_ov000_0236ac50.unk0;
        w->mdl8F4 = data_ov000_0236ac50.unk8;
        w->anm8F8 = data_ov000_0236ac40;
        w->anm8FC = data_ov000_0236ac48;
        w->anm900 = func_02021264(buf1, 0);
        w->anm904 = func_02021264(buf1, 1);
        func_020212e0(buf1);
    } else {
        res = File_Load(data_ov000_0236930c, (void *)-1);
        w->unk908 = UnkAlloc_0235dab4(*(u32 *)res >> 8, 0x20);
        dst = w->unk908;
        switch (func_ov000_0235d43c(res)) {
        case 0x10:
            MI_UncompressLZ16(res, dst);
            break;
        case 0x20:
            MI_UncompressHuffman(res, dst);
            break;
        case 0x30:
            MI_UncompressRL16(res, dst);
            break;
        default:
            Mem_Copy(Compress_GetData(res), dst, func_ov000_0235d430(res));
            break;
        }
        DC_StoreRange(dst, func_ov000_0235d430(res));
        UnkFree_0235dab4(res);
        func_02021310(buf0, data_ov000_02369324, w->unk908);
        func_02021310(buf1, data_ov000_02369328, w->unk90C);
        func_02021264(buf0, 0);
        NNS_G3dResDefaultSetup();
        w->mdl8F0 = func_02021264(buf0, 0);
        w->anm8F8 = func_02021264(buf0, 1);
        w->anm8FC = func_02021264(buf0, 2);
        w->anm900 = func_02021264(buf1, 0);
        w->anm904 = func_02021264(buf1, 1);
        func_02021264(buf0, 3);
        NNS_G3dResDefaultSetup();
        w->mdl8F4 = func_02021264(buf0, 3);
        func_020212e0(buf1);
        func_020212e0(buf0);
    }
    Model_Init(&w->rA, 0);
    Model_SetResource(&w->rA, w->mdl8F0, 0);
    Model_SetAnimation(&w->rA, 0, w->anm8F8, 0, NULL);
    Model_SetAnimation(&w->rA, 4, w->anm8FC, 1, NULL);
    Model_Init(&w->rB, 0);
    Model_SetResource(&w->rB, w->mdl8F4, 0);
    Model_SetAnimation(&w->rB, 0, w->anm8F8, 0x37, NULL);
    Model_SetAnimation(&w->rB, 4, w->anm8FC, 1, NULL);
    w->flagA |= 4;
    w->flagB |= 4;
    G3X_SetEdgeColorTable(data_ov000_02364ed0);
    func_02022270(0, data_ov000_02364ec8[0]);
    func_02022270(1, data_ov000_02364ec8[1]);
    func_02022270(2, data_ov000_02364ec8[2]);
}

#pragma thumb reset
