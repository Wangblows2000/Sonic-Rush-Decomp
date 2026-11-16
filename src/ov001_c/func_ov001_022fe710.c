#include <nitro/types.h>

typedef struct UnkStruct_022fe710_o {
    u8 pad0[4];
    s32 type;
    u8 pad8[0x14 - 8];
    u32 flags;
    u8 pad18[0x98 - 0x18];
    u16 hp;
} UnkStruct_022fe710_o;

typedef struct UnkStruct_022fe710_c {
    UnkStruct_022fe710_o *owner;
    void (*fn)(struct UnkStruct_022fe710_c *a, struct UnkStruct_022fe710_c *b);
    u32 flags;
} UnkStruct_022fe710_c;

extern s32 data_ov001_0231f334[];
extern s32 data_ov001_0231f328[];
extern UnkStruct_022fe710_c *data_ov001_0231f430[][20];
extern UnkStruct_022fe710_c *data_ov001_0231f340[][20];

BOOL func_ov001_022fe6b8(UnkStruct_022fe710_c *a, UnkStruct_022fe710_c *b);

void func_ov001_022fe710(s32 k) {
    s32 nA = data_ov001_0231f334[k];
    s32 i;
    s32 j;
    s32 nB = data_ov001_0231f328[k];
    UnkStruct_022fe710_c **pa = data_ov001_0231f430[k];
    UnkStruct_022fe710_c **pb;
    UnkStruct_022fe710_c *a;
    UnkStruct_022fe710_c *b;

    for (i = 0; i < nA; i++, pa++) {
        for (pb = data_ov001_0231f340[k], j = 0; j < nB; j++, pb++) {
            a = *pa;
            b = *pb;
            if (a->owner->flags & 0x1000)
                continue;
            if (b->owner->flags & 0x1000)
                continue;
            if (a->owner->flags & 8)
                continue;
            if (b->owner->flags & 8)
                continue;
            if (a->flags & 0x200)
                continue;
            if (b->flags & 0x200)
                continue;
            if (!func_ov001_022fe6b8(a, b))
                continue;
            if (a->fn != NULL) {
                a->fn(a, b);
            } else if (a->owner->type == 1 && a->owner->hp != 0xffff) {
                a->owner->hp--;
                if (a->owner->hp == 0)
                    a->owner->flags |= 8;
            }
            if (b->fn != NULL) {
                b->fn(a, b);
            } else if (b->owner->type == 1 && a->owner->hp != 0xffff) {
                b->owner->hp--;
                if (b->owner->hp == 0)
                    b->owner->flags |= 8;
            }
        }
    }
}
