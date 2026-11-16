#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    u8 pad1c[0x14];
    void *src;
} UnkStruct_022ea1b8;

extern void *data_02250438;
extern void *data_02250434;

s32 func_ov001_022ea270(void *src);
u32 func_ov001_022ea264(void *src);
void func_02008648(void *src, void *dst);
void func_02008720(void *src, void *dst);
void func_020087f8(void *src, void *dst);
void *func_02033a54(void *src);
void func_02033a68(void *src, void *dst, u32 size);
void DC_StoreRange(const void *p, u32 size);
void func_02035704(void *p, u16 a, s32 b, s32 c);
void func_02035754(void);

#pragma thumb on
static inline void LoadPal_022ea1b8(void *src, u16 n, s32 a, s32 b) {
    void *s0 = data_02250438;
    void *s1 = data_02250434;
    data_02250438 = NULL;
    data_02250434 = NULL;
    func_02035704(src, n, a, b);
    func_02035754();
    data_02250438 = s0;
    data_02250434 = s1;
}

void func_ov001_022ea1b8(UnkStruct_022ea1b8 *p, s32 start, s32 end) {
    void *src = p->src;
    u16 buf[0x100];

    switch (func_ov001_022ea270(src)) {
    case 0x10:
        func_02008648(src, buf);
        break;
    case 0x20:
        func_02008720(src, buf);
        break;
    case 0x30:
        func_020087f8(src, buf);
        break;
    default:
        func_02033a68(func_02033a54(src), buf, func_ov001_022ea264(src));
        break;
    }
    DC_StoreRange(buf, func_ov001_022ea264(src));
    LoadPal_022ea1b8(&buf[start], end - start + 1, p->unk14, p->unk18);
}
#pragma thumb reset
