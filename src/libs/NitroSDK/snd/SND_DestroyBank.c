#include <nitro/types.h>

struct UnkObj_02009e2c;

typedef struct UnkEntry_02009e2c {
    struct UnkObj_02009e2c *owner;
    struct UnkEntry_02009e2c *next;
} UnkEntry_02009e2c;

typedef struct UnkObj_02009e2c {
    u8 pad0[0x18];
    UnkEntry_02009e2c entries[4];
    u8 pad38[4];
} UnkObj_02009e2c;

#define HEAD(p) (*(UnkEntry_02009e2c **)&(p)->entries[0].owner)

extern void SNDi_LockMutex(void);
extern void SNDi_UnlockMutex(void);
extern void DC_StoreRange(const void *p, u32 size);

void SND_DestroyBank(UnkObj_02009e2c *obj)
{
    s32 i;
    SNDi_LockMutex();
    for (i = 0; i < 4; i++) {
        UnkObj_02009e2c *p = obj->entries[i].owner;
        if (p != NULL) {
            UnkEntry_02009e2c *q = HEAD(p);
            if (&obj->entries[i] == q) {
                HEAD(p) = obj->entries[i].next;
                DC_StoreRange(p, sizeof(UnkObj_02009e2c));
            } else {
                while (q != NULL) {
                    if (&obj->entries[i] == q->next) {
                        break;
                    }
                    q = q->next;
                }
                q->next = obj->entries[i].next;
                DC_StoreRange(q, sizeof(UnkEntry_02009e2c));
            }
        }
    }
    SNDi_UnlockMutex();
}
