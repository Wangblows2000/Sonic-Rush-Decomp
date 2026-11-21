#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa];
    u8 unka;
    u8 unkb;
    u8 padc[0xe8];
    void *res;
    void *owner;
    u32 flags;
} UnkSprite_02069934;

typedef struct {
    u8 pad0[0x54];
    u32 flags;
    u8 pad58[0x1c];
    s32 unk74;
    u8 pad78[0xb4];
    UnkSprite_02069934 *sprite;
} UnkWork_02069934;

void *ObjData_Load(void *s, const char *name, void *arc);
u32 VramTex_Alloc(u32 size, BOOL flag);
u32 VramPltt_Alloc(u32 size, BOOL flag);
void Model_InitSprite3D(UnkSprite_02069934 *s, void *res, s32 a, s32 b, u32 vram, u32 pal, s32 c, UnkWork_02069934 *w);

void StageObj_Load3dBAC(UnkWork_02069934 *w, UnkSprite_02069934 *s, const char *name, u32 size, u16 palSize, void *owner, void *arc) {
    void *res;
    u32 vram = 0;
    u32 pal = 0;

    w->sprite = s;
    if (arc != NULL)
        s->flags |= 1;
    s->owner = owner;
    res = ObjData_Load(owner, name, arc);
    if (res == NULL && arc != NULL) {
        w->flags &= ~0x100;
        res = ObjData_Load(s->owner, name, NULL);
    }
    s->res = res;
    if (size != 0) {
        vram = VramTex_Alloc(size, FALSE);
        if (palSize == 0)
            palSize = 0x10;
        pal = VramPltt_Alloc(palSize, FALSE);
    }
    Model_InitSprite3D(s, res, 0, 0, vram, pal, w->unk74, w);
    s->unka = 0x13;
    s->unkb = 6;
}
