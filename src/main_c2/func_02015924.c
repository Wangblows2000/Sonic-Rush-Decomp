#include <nitro/types.h>

typedef struct {
    u32 src;
    u32 addr;
    u32 size;
    u32 flag : 1;
    u32 unkC : 31;
} UnkMbSeg_02015924;

typedef struct {
    u32 offset[3]; /* 0x00 */
    u16 blk[3];    /* 0x0c */
    u16 total;     /* 0x12 */
} UnkMbSegInfo_02015924;

typedef struct {
    u32 src;
    u32 size;
    u32 offset;
    u8 seg;
} UnkMbSegPos_02015924;

typedef struct {
    u32 base;
    u32 size;
    u8 *ptr;
    u32 state;
} UnkBlock_02015924;

typedef struct {
    u32 unk0;
    u8 pad4[0x2c];
    UnkBlock_02015924 blocks[4]; /* 0x30 */
} UnkCache_02015924;

typedef struct {
    u8 pad0[0x5a4];
    UnkMbSegInfo_02015924 segInfo; /* 0x5a4 */
    u8 pad5b8[8];
    u16 unk5c0;
    u16 unk5c2;
    u16 unk5c4;
    u16 unk5c6;
    u16 unk5c8;
    u8 unk5ca;
    u8 pad5cb;
    UnkCache_02015924 *cache;  /* 0x5cc */
    u32 *base;                 /* 0x5d0 */
} UnkEntry_02015924;

typedef struct {
    u8 pad0[0x10];
    UnkBlock_02015924 *req;
    UnkCache_02015924 *arc;
} UnkTask_02015924;

typedef struct {
    u8 pad0[0x1524];
    u8 unk1524;                 /* 0x1524 */
    u8 unk1525;                 /* 0x1525 */
    u8 pad1526[0x1788 - 0x1526];
    UnkEntry_02015924 entry[16]; /* 0x1788 */
    u8 pad74c8[0x7ce0 - 0x74c8];
    UnkTask_02015924 task;      /* 0x7ce0 */
} UnkWork_02015924;

typedef struct {
    u8 type;
    u8 pad1;
    u16 val;
    u16 val2;
} UnkPacket_02015924;

extern UnkWork_02015924 *data_02080f1c;

#define ENTRY_02015924(k) (((UnkWork_02015924 *)((u8 *)data_02080f1c + (k) * sizeof(UnkEntry_02015924)))->entry[0])

void func_02015728(s32 k);
BOOL func_02016d5c(UnkMbSegPos_02015924 *out, UnkMbSegInfo_02015924 *info, u32 idx, void *h);
u8 *func_0201a8ac(const UnkPacket_02015924 *pkt, void *dst);
BOOL func_0201a16c(UnkCache_02015924 *s, u32 addr, void *dest, u32 size);
s32 func_020179dc(u32 a, u16 bitmap, void *p);
BOOL func_0201a478(UnkTask_02015924 *t);
void OS_Terminate(void);
void func_0201a2e8(UnkTask_02015924 *task, void (*func)(UnkTask_02015924 *), void (*done)(UnkTask_02015924 *), u32 priority);
void func_02015bdc(UnkTask_02015924 *t);

s32 func_02015924(void)
{
    u8 i;
    UnkMbSegPos_02015924 pos;
    UnkPacket_02015924 pkt;
    u8 *p;
    u32 addr;
    UnkCache_02015924 *cache;

    if (data_02080f1c->unk1524 == 0) {
        return 0x15;
    }
    for (i = 0; i < 16; i++) {
        data_02080f1c->unk1525 = (data_02080f1c->unk1525 + 1) % 16;
        if (ENTRY_02015924(data_02080f1c->unk1525).unk5ca != 0 && ENTRY_02015924(data_02080f1c->unk1525).unk5c4 != 0) {
            break;
        }
    }
    if (i == 16) {
        return 0x15;
    }
    func_02015728(data_02080f1c->unk1525);
    {
        UnkWork_02015924 *w = data_02080f1c;
        u8 k = w->unk1525;
        if (!func_02016d5c(&pos, &w->entry[k].segInfo, w->entry[k].unk5c0, &w->entry[k])) {
            return 0x15;
        }
    }
    pkt.type = 4;
    pkt.val = data_02080f1c->unk1525;
    pkt.val2 = ENTRY_02015924(data_02080f1c->unk1525).unk5c0;
    p = func_0201a8ac(&pkt, data_02080f1c);
    addr = pos.offset - ENTRY_02015924(data_02080f1c->unk1525).segInfo.offset[pos.seg] + ENTRY_02015924(data_02080f1c->unk1525).base[pos.seg];
    cache = ENTRY_02015924(data_02080f1c->unk1525).cache;
    if (!func_0201a16c(cache, addr, p, pos.size)) {
        UnkTask_02015924 *task = &data_02080f1c->task;
        if (!func_0201a478(task) && cache->unk0) {
            UnkBlock_02015924 *blocks = cache->blocks;
            UnkBlock_02015924 *best = NULL;
            s32 j;
            for (j = 0; j < 4; j++) {
                if (blocks[j].state == 2 && (best == NULL || best->base > blocks[j].base)) {
                    best = &blocks[j];
                }
            }
            if (best == NULL) {
                OS_Terminate();
            }
            cache->unk0 = 0;
            best->state = 1;
            best->base = addr & ~0x1f;
            task->req = best;
            task->arc = cache;
            func_0201a2e8(task, func_02015bdc, NULL, 4);
        }
        return 0x15;
    } else {
        return func_020179dc(pos.size + 6, ENTRY_02015924(data_02080f1c->unk1525).unk5c4, data_02080f1c);
    }
}
