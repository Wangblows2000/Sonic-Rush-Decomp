#include <nitro/types.h>

struct UnkObj_02009ecc;

typedef struct UnkEntry_02009ecc {
    struct UnkObj_02009ecc *owner;
    struct UnkEntry_02009ecc *next;
} UnkEntry_02009ecc;

typedef struct UnkObj_02009ecc {
    u8 pad0[0x18];
    UnkEntry_02009ecc entries[4];
    u8 pad38[4];
} UnkObj_02009ecc;

#define HEAD(p) (*(UnkEntry_02009ecc **)&(p)->entries[0].owner)

extern void SNDi_LockMutex(void);
extern void SNDi_UnlockMutex(void);
extern void DC_StoreRange(const void *p, u32 size);

void SND_AssignWaveArc(UnkObj_02009ecc *obj, s32 idx, UnkObj_02009ecc *parent)
{
    UnkObj_02009ecc *p;
    SNDi_LockMutex();
    p = obj->entries[idx].owner;
    if (p != NULL) {
        UnkEntry_02009ecc *q;
        if (parent == p) {
            SNDi_UnlockMutex();
            return;
        }
        q = HEAD(p);
        if (&obj->entries[idx] == q) {
            HEAD(p) = obj->entries[idx].next;
            DC_StoreRange(obj->entries[idx].owner, sizeof(UnkObj_02009ecc));
        } else {
            while (q != NULL) {
                if (&obj->entries[idx] == q->next) {
                    break;
                }
                q = q->next;
            }
            q->next = obj->entries[idx].next;
            DC_StoreRange(q, sizeof(UnkEntry_02009ecc));
        }
    }
    {
        UnkEntry_02009ecc *old = HEAD(parent);
        HEAD(parent) = &obj->entries[idx];
        obj->entries[idx].next = old;
        obj->entries[idx].owner = parent;
    }
    SNDi_UnlockMutex();
    DC_StoreRange(obj, sizeof(UnkObj_02009ecc));
    DC_StoreRange(parent, sizeof(UnkObj_02009ecc));
}
