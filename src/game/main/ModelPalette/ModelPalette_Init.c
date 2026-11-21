#include <nitro/types.h>

u8 *Heap_Alloc(void *heap, u32 size, s32 align);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);

typedef struct {
    u16 *dst;
    u16 *src;
    void *res;
} UnkPal_0206cf8c;

extern void *gMainHeap;
extern u32 data_0208a764[];

u32 GX_ResetBankForTexPltt(void);
void GX_SetBankForTexPltt(u32 e);
void *NNS_G3dGetTex(void *a);
u32 NNS_G3dPlttGetRequiredSize(void *res);
u32 Asset3D_GetPaletteKeyByIndex(void *res, s32 a);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);

static inline u8 *Alloc_0206cf8c(u32 sz) {
    u32 size = sz;
    u8 *p = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

void ModelPalette_Init(void *a, UnkPal_0206cf8c *p) {
    u32 e;
    u32 bank;
    void *addr;

    e = GX_ResetBankForTexPltt();
    p->res = NNS_G3dGetTex(a);
    p->dst = (u16 *)Alloc_0206cf8c((NNS_G3dPlttGetRequiredSize(p->res) + 0x43) & ~0x1f);
    p->src = (u16 *)Alloc_0206cf8c((NNS_G3dPlttGetRequiredSize(p->res) + 0x43) & ~0x1f);
    bank = Asset3D_GetPaletteKeyByIndex(p->res, 0) >> 14;
    addr = (void *)(data_0208a764[bank] + Asset3D_GetPaletteKeyByIndex(p->res, 0) - (bank << 14));
    MIi_CpuCopy16(addr, p->src, NNS_G3dPlttGetRequiredSize(p->res));
    GX_SetBankForTexPltt(e);
}
