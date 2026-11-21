#ifndef OV000_B_RES_H
#define OV000_B_RES_H

#include <nitro/types.h>

/* offset-based resource block: data at (u8 *)res + unk18, 0x14-byte entries from +0xC */
typedef struct {
    u8 pad0[0x18];
    s32 unk18;
} UnkStruct_Res;

typedef struct {
    u8 pad0[0x12];
    u16 unk12;
} UnkStruct_ResElem;

typedef struct {
    u8 pad0[0xC];
    UnkStruct_ResElem elems[1];
} UnkStruct_ResHdr;

static inline u16 Res_GetElemVal(UnkStruct_Res *r, u16 idx) {
    UnkStruct_ResElem *e = ((UnkStruct_ResHdr *)((u8 *)r + r->unk18))->elems;
    return e[idx].unk12;
}

#endif
