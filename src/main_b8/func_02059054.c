#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
} UnkSave_02059054;

typedef struct {
    u8 pad0[2];
    s8 slot;
} UnkData_02059054;

extern void *data_0208fe78;
extern UnkData_02059054 data_022c4994;

u32 func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);
u8 *func_02034a40(void *heap, u32 size, s32 align);
BOOL func_02058b50(u8 idx, UnkSave_02059054 *buf);
BOOL func_02058898(UnkSave_02059054 *buf);

static inline void Free_02059054(u8 *p) {
    if ((u8 *)func_02006abc(0) != p) {
        u32 size;
        p -= 0x20;
        size = *(u32 *)p;
        if (*(u32 *)(p + size - 4) == 0x02467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe78, p);
}

static inline u8 *Alloc_02059054(u32 sz) {
    u32 size = sz;
    u8 *p = func_02034a40(data_0208fe78, size, 0x20);
    if ((u8 *)func_02006abc(0) != p) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, p, size);
        *(u32 *)p = size;
        *(u32 *)(p + size - 4) = 0x02467531;
        DC_StoreRange(p, size);
        p += 0x20;
    }
    return p;
}

BOOL func_02059054(void) {
    s8 best;
    u8 i;
    u32 cnt[5] = {0, 0, 0, 0, 0};
    UnkSave_02059054 *buf;
    u8 st[5] = {0, 0, 0, 0, 0};
    BOOL bad;
    BOOL zero;

    buf = (UnkSave_02059054 *)Alloc_02059054(0x340);
    if ((u8 *)buf == (u8 *)func_02006abc(0))
        return FALSE;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, buf, 0x314);
    }
    for (i = 0; i < 5; i++) {
        bad = FALSE;
        if (func_02058b50(i * 2, buf)) {
            if (func_02058898(buf)) {
                cnt[i] = buf->unk4;
                continue;
            }
            bad = TRUE;
            cnt[i] = buf->unk4;
        }
        if (func_02058b50(i * 2 + 1, buf)) {
            if (func_02058898(buf)) {
                cnt[i] = buf->unk4;
            } else {
                st[i] = 2;
                cnt[i] = buf->unk4;
            }
        } else if (bad) {
            st[i] = 2;
        } else {
            st[i] = 1;
        }
    }
    Free_02059054((u8 *)buf);
    best = -1;
    for (i = 0; i < 5; i++) {
        if (st[i] == 1)
            continue;
        if (best < 0)
            best = i;
        else if (cnt[i] > cnt[best])
            best = i;
    }
    if (best < 0)
        return FALSE;
    zero = TRUE;
    for (i = 0; i < 5; i++) {
        if (cnt[i] != 0) {
            zero = FALSE;
            break;
        }
    }
    if (zero)
        best = 0;
    data_022c4994.slot = best;
    if (st[best] == 2)
        return FALSE;
    return (cnt[best] - best) % 5 == 0;
}
