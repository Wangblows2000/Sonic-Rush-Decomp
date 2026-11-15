#include <nitro/types.h>
#include <main_sdk2/wh.h>

typedef struct {
    u8 type;
    u8 pad1;
    u16 val;
    u16 val2;
} UnkPkt_02015c78;

extern u8 data_020779b4;

#define ENTRY_02015c78(k) (((UnkStruct_02080f1c *)((u8 *)data_02080f1c + (k) * sizeof(UnkEntry1788_wh)))->unk1788[0])

void MI_CpuFill8(void *dest, u8 data, u32 size);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
u8 *func_0201a8ac(const UnkPkt_02015c78 *pkt, void *dst);
s32 func_020179dc(u32 a, u16 bitmap, void *p);

s32 func_02015c78(void)
{
    UnkPkt_02015c78 pkt;
    u8 cnt[16];
    s8 found = -1;
    u16 bitmap = 0;
    UnkStruct_02080f1c *p;
    u16 i;
    u8 j;
    u8 k;
    u8 *q;

    MI_CpuFill8(cnt, 0, sizeof(cnt));
    p = data_02080f1c;
    for (i = 1; i <= 15; i++) {
        if (p->unk14e8[i - 1] == 5) {
            cnt[p->unk1526[i - 1]]++;
        }
    }
    k = data_020779b4;
    for (j = 0; j < 16; j++) {
        k = (k + 1) % 16;
        if (ENTRY_02015c78(k).unk5ca != 0 && cnt[k] != 0) {
            found = k;
            break;
        }
    }
    if (found == -1) {
        return 0x15;
    }
    data_020779b4 = found;
    for (i = 1; i <= 15; i++) {
        if (p->unk14e8[i - 1] == 5 && p->unk1526[i - 1] == found) {
            bitmap |= 1 << i;
        }
    }
    pkt.type = 3;
    pkt.val = found;
    q = func_0201a8ac(&pkt, p);
    if (q != NULL) {
        MI_CpuCopy8(&data_02080f1c->unk1788[found], q, 0xe4);
    }
    return func_020179dc(0xea, bitmap, data_02080f1c);
}
