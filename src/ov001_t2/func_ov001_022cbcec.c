#include <nitro/types.h>

typedef struct {
    s32 type;
    u8 pad4[0x2c];
} UnkItem_022cbcec;

typedef struct UnkStruct_022cbcec {
    u8 pad0[4];
    s32 next;
    s32 cur;
    u8 padc[0x28c - 0xc];
    UnkItem_022cbcec items[1];
} UnkStruct_022cbcec;

extern void (*data_ov001_0231a6bc[])(UnkStruct_022cbcec *p);
extern void (*data_ov001_0231a68c[])(UnkStruct_022cbcec *p);
extern void (*data_ov001_0231a674[])(UnkStruct_022cbcec *p, s16 x, s16 y);

#pragma thumb on
void func_ov001_022cbcec(UnkStruct_022cbcec *p, s32 x, s32 y) {
    if (p->next != p->cur) {
        if (p->cur >= 0) {
            data_ov001_0231a6bc[p->items[p->cur].type](p);
        }
        p->cur = p->next;
        data_ov001_0231a68c[p->items[p->next].type](p);
    }
    data_ov001_0231a674[p->items[p->next].type](p, x, y);
}
#pragma thumb reset
