#include <nitro/types.h>

typedef struct {
    u8 unk0[8];
    u32 vramKey;
    u16 sizeTex;
    u16 unkE;
    u16 flag;
    u16 unk12;
    u32 ofsTex;
    u32 vramKey4x4;
    u16 sizeTex4x4;
    u16 unk1E;
    u16 flag4x4;
    u16 unk22;
    u32 ofsTex4x4;
    u32 ofsTexPlttIdx;
} UnkTex_02021b1c;

void GX_BeginLoadTex(void);
void GX_LoadTex(const void *src, u32 dest, u32 size);
void GX_EndLoadTex(void);

void NNS_G3dTexLoad(UnkTex_02021b1c *tex, BOOL lock) {
    u32 sz2;
    u32 sz;
    if (lock) GX_BeginLoadTex();
    sz = tex->sizeTex << 3;
    if (sz != 0) {
        GX_LoadTex((u8 *)tex + tex->ofsTex, (tex->vramKey & 0xFFFF) << 3, sz);
        tex->flag |= 1;
    }
    sz2 = tex->sizeTex4x4 << 3;
    if (sz2 != 0) {
        u8 *pltt;
        u32 addr;
        addr = (tex->vramKey4x4 & 0xFFFF) << 3;
        pltt = (u8 *)tex + tex->ofsTexPlttIdx;
        GX_LoadTex((u8 *)tex + tex->ofsTex4x4, addr, sz2);
        GX_LoadTex(pltt, 0x20000 + ((addr & 0x1FFFF) >> 1) + ((addr & 0x40000) >> 2), sz2 >> 1);
        tex->flag4x4 |= 1;
    }
    if (lock) GX_EndLoadTex();
}
