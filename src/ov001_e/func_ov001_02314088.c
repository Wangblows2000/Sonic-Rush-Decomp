#include <nitro/types.h>

typedef struct UnkStruct_02314088_e {
    s32 type;
    u8 a;
    u8 b;
    u16 c;
    s32 d;
    s32 e;
} UnkStruct_02314088_e;

typedef struct UnkStruct_02314088_s {
    u8 pad0[8];
    UnkStruct_02314088_e *entries;
    s32 count;
} UnkStruct_02314088_s;

typedef struct UnkStruct_02314088 {
    u8 pad0[0x10];
    s32 unk10;
    UnkStruct_02314088_s *script;
    s32 cur;
} UnkStruct_02314088;

s32 func_ov001_0230db44(u8 a);
void *func_ov001_0230c090(void);
void func_ov001_0230f550(void *a, s32 b, u8 c, u8 d, s32 e, u16 f, s32 g, s32 h);

BOOL func_ov001_02314088(UnkStruct_02314088 *p) {
    UnkStruct_02314088_e *tbl = p->script->entries;
    UnkStruct_02314088_e *e;

    do {
        e = &tbl[p->cur];
        switch (e->type) {
        case 0:
            p->unk10 = func_ov001_0230db44(e->a);
            break;
        case 1:
            func_ov001_0230f550(func_ov001_0230c090(), p->unk10, e->a, e->b, 0, e->c, e->d, e->e);
            break;
        }
        p->cur++;
        if (p->cur >= p->script->count)
            return FALSE;
    } while (tbl[p->cur].type != 0);
    return TRUE;
}
