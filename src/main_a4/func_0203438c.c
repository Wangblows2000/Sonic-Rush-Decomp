#include <nitro/types.h>

typedef struct {
    u32 flags;
    u32 x;
    u32 y;
    u32 tblIdx;
    u8 slot;
    u8 pad11[3];
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    s32 unk20;
    u16 unk24;
    u16 unk26;
    u16 unk28;
    u16 unk2A;
    u16 unk2C;
    u16 unk2E;
    u32 unk30;
    u32 unk34;
    void *unk38;
    u16 unk3C;
} UnkStruct_0203438c;

extern u32 data_02250438;
extern u32 data_02250434;
extern u32 data_0208f7e0;
extern u32 data_0208f7dc;
extern u32 data_02250440;
extern u32 data_0225043c;
typedef struct { u16 x; u16 y; } UnkPos_0203438c;
extern UnkPos_0203438c *data_02072c78[];

void func_020356bc(u32 a, u32 b, u32 c);
void func_02035754(void);
void func_020337a4(u32 a, u32 b, u32 c);
void func_0203383c(void);
void func_02035d04(void *src, u16 x, u16 y, u16 a, u32 flip, s32 mode, u16 c, u16 d, u16 e, u16 f, u16 w, u16 h);
void func_020363f0(void);

static inline void Imm_020356bc(u32 a, u32 b, u32 c)
{
    u32 head = data_02250438;
    u32 tail = data_02250434;
    data_02250438 = 0;
    data_02250434 = 0;
    func_020356bc(a, b, c);
    func_02035754();
    data_02250438 = head;
    data_02250434 = tail;
}

static inline void Imm_020337a4(u32 a, u32 b, u32 c)
{
    u32 head = data_0208f7e0;
    u32 tail = data_0208f7dc;
    data_0208f7e0 = 0;
    data_0208f7dc = 0;
    func_020337a4(a, b, c);
    func_0203383c();
    data_0208f7e0 = head;
    data_0208f7dc = tail;
}

static inline void Imm_02035d04(void *src, u16 x, u16 y, u16 a, u32 flip, s32 mode, u16 c, u16 d, u16 e, u16 f, u16 w, u16 h)
{
    u32 head = data_02250440;
    u32 tail = data_0225043c;
    data_02250440 = 0;
    data_0225043c = 0;
    func_02035d04(src, x, y, a, flip, mode, c, d, e, f, w, h);
    func_020363f0();
    data_02250440 = head;
    data_0225043c = tail;
}

void func_0203438c(UnkStruct_0203438c *s)
{
    u32 flags;
    u16 w, h;

    if (s->unk30 != 0 && !(s->flags & 1)) {
        if (s->flags & 8)
            Imm_020356bc(s->unk30, s->unk14, s->unk18);
        else
            func_020356bc(s->unk30, s->unk14, s->unk18);
    }
    if (s->unk34 != 0 && !(s->flags & 2)) {
        if (s->flags & 0x10)
            Imm_020337a4(s->unk34, 0, s->unk1C);
        else
            func_020337a4(s->unk34, 0, s->unk1C);
    }
    if (s->unk38 == NULL)
        return;
    flags = s->flags;
    if (flags & 4)
        return;
    w = s->unk2C;
    h = s->unk2E;
    if (flags & 0x400) {
        w++;
        h++;
    }
    if (flags & 0x20)
        Imm_02035d04(s->unk38, s->x >> 3, s->y >> 3, s->unk3C, flags & 0x1000, s->unk20, s->unk24, s->unk26, s->unk28, s->unk2A, w, h);
    else
        func_02035d04(s->unk38, s->x >> 3, s->y >> 3, s->unk3C, flags & 0x1000, s->unk20, s->unk24, s->unk26, s->unk28, s->unk2A, w, h);
    if (s->flags & 0x800) {
        UnkPos_0203438c *tbl = data_02072c78[s->tblIdx] + s->slot;
        tbl->x = (s->unk28 << 3) + (s->x & 7);
        tbl->y = (s->unk2A << 3) + (s->y & 7);
    }
}
