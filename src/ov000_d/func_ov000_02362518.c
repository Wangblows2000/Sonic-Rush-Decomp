#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1A;
} UnkStruct_02362518_Res;

typedef struct {
    u8 pad0[0x750];
    u8 bg750[0x40];
    u8 bg790[0x40];
    u8 bg7D0[0x40];
    u8 bg810[0x102C - 0x810];
    UnkStruct_02362518_Res *res102C;
    UnkStruct_02362518_Res *res1030;
    UnkStruct_02362518_Res *res1034;
} UnkStruct_02362518;

extern void *data_0208fe78;

void func_0203473c(void *bg, UnkStruct_02362518_Res *res, s32 a, s32 b, s32 c, u16 w, u16 h);
void func_0203438c(void *bg);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_020349b8(void *heap, void *p);

static inline void UnkFree_02362518(void *p) {
    u32 size;
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    func_020349b8(data_0208fe78, p);
}

#pragma thumb on

void func_ov000_02362518(UnkStruct_02362518 *w) {
    func_0203473c(w->bg810, w->res1030, 0x38, 0, 1, w->res1030->unk18, w->res1030->unk1A);
    func_0203438c(w->bg810);
    func_0203473c(w->bg750, w->res102C, 0x38, 0, 2, w->res102C->unk18, w->res102C->unk1A);
    func_0203438c(w->bg750);
    func_0203473c(w->bg7D0, w->res1034, 0x38, 1, 1, w->res1034->unk18, w->res1034->unk1A);
    func_0203438c(w->bg7D0);
    func_0203473c(w->bg790, w->res102C, 0x38, 1, 2, w->res102C->unk18, w->res102C->unk1A);
    func_0203438c(w->bg790);
    UnkFree_02362518(w->res1030);
    UnkFree_02362518(w->res102C);
    UnkFree_02362518(w->res1034);
    w->res1030 = NULL;
    w->res102C = NULL;
    w->res1034 = NULL;
}

#pragma thumb reset
