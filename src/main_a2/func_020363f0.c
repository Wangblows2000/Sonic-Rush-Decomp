#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 unk4[1];
} UnkScr_020363f0;

typedef struct UnkJob_020363f0 {
    struct UnkJob_020363f0 *next;
    u16 type;
    u16 unk6;
    u32 unk8;
    u32 unkC;
    u16 unk10;
    u16 unk12;
    UnkScr_020363f0 *src;
    u32 unk18;
    u32 unk1c;
} UnkJob_020363f0;

extern UnkJob_020363f0 *data_02250440;
extern UnkJob_020363f0 *data_0225043c;

void func_02035b5c(u32 src, u32 srcStride, u32 dst, u32 dstStride, u16 size, u16 rows);
u32 func_02036590(void *src);
u32 func_02036584(void *src);
u32 func_02033a54(void *src);
void func_02033a68(u32 src, u32 dst, u32 size);
void func_02008648(void *src, u32 dst);
void func_02008720(void *src, u32 dst);
void func_020087f8(void *src, u32 dst);
void DC_StoreRange(u32 addr, u32 size);
void func_02037e68(UnkJob_020363f0 *job);

void func_020363f0(void) {
    while (data_02250440 != NULL) {
        UnkJob_020363f0 *j = data_02250440;
        switch (j->type) {
        case 0:
            func_02035b5c((u32)j->src + j->unk1c, j->unk18, j->unkC + j->unk8, j->unk6, j->unk10, j->unk12);
            break;
        case 1: {
            UnkScr_020363f0 *src = j->src;
            if (src->unk0 & 0xf0) {
                u32 dst = j->unkC + j->unk8;
                switch (func_02036590(src)) {
                case 0x10:
                    func_02008648(src, dst);
                    break;
                case 0x20:
                    func_02008720(src, dst);
                    break;
                case 0x30:
                    func_020087f8(src, dst);
                    break;
                default:
                    func_02033a68(func_02033a54(src), dst, func_02036584(src));
                    break;
                }
                DC_StoreRange(dst, func_02036584(src));
            } else {
                func_02035b5c(j->unk1c + (u32)src->unk4, j->unk18, j->unkC + j->unk8, j->unk6, j->unk10, j->unk12);
            }
            break;
        }
        }
        func_02037e68(data_02250440);
        data_02250440 = data_02250440->next;
        if (data_02250440 == NULL) {
            data_0225043c = NULL;
        }
    }
}
