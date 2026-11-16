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

void *func_020687b8(void *s, const char *name, void *arc);
u32 func_0203969c(u32 size, BOOL flag);
u32 func_02039428(u32 size, BOOL flag);
void func_02031878(UnkSprite_02069934 *s, void *res, s32 a, s32 b, u32 vram, u32 pal, s32 c, UnkWork_02069934 *w);

void func_02069934(UnkWork_02069934 *w, UnkSprite_02069934 *s, const char *name, u32 size, u16 palSize, void *owner, void *arc) {
    void *res;
    u32 vram = 0;
    u32 pal = 0;

    w->sprite = s;
    if (arc != NULL)
        s->flags |= 1;
    s->owner = owner;
    res = func_020687b8(owner, name, arc);
    if (res == NULL && arc != NULL) {
        w->flags &= ~0x100;
        res = func_020687b8(s->owner, name, NULL);
    }
    s->res = res;
    if (size != 0) {
        vram = func_0203969c(size, FALSE);
        if (palSize == 0)
            palSize = 0x10;
        pal = func_02039428(palSize, FALSE);
    }
    func_02031878(s, res, 0, 0, vram, pal, w->unk74, w);
    s->unka = 0x13;
    s->unkb = 6;
}
