#include <nitro/types.h>
#include <NitroSDK/wh.h>
#include <NitroSDK/blk2.h>

typedef struct {
    u32 ggid;
    UnkInitParam_wh info;
    u16 unk1a;
    u8 group;
} UnkJoinReq_02015f14;

void *MBi_SetRecvBufferFromChild(const u8 *src, UnkPacket_0201a770 *pkt, u16 aid);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MBi_CommChangeParentState(u16 aid, u32 type, void *arg);
u16 MBi_calc_nextsendblock(u16 a, u16 b);

#define ENTRY_02015f14(k) (((UnkStruct_02080f1c *)((u8 *)data_02080f1c + (k) * sizeof(UnkEntry1788_wh)))->unk1788[0])

static inline void func_02015f14_Join(u16 aid, u8 k)
{
    u32 bit = 1 << aid;
    if (data_02080f1c->unk1536 & bit) {
        return;
    }
    data_02080f1c->unk1535++;
    data_02080f1c->unk1536 |= bit;
    data_02080f1c->unk1526[aid - 1] = k;
    ENTRY_02015f14(k).unk5c6 |= bit;
    ENTRY_02015f14(k).unk5c8 |= bit;
    data_02080f1c->unk1754[aid - 1] = 0;
    MBi_CommChangeParentState(aid, 5, NULL);
}

static inline void func_02015f14_Req(u16 aid, s32 idx, UnkJoinReq_02015f14 *src)
{
    UnkJoinReq_02015f14 req;
    MI_CpuCopy8(src, &req, 0x1d);
    data_02080f1c->unk14a8[idx] = req.ggid;
    data_02080f1c->unk148a[idx] = req.unk1a;
    MI_CpuCopy8(&req.info, &data_02080f1c->childInit[idx], sizeof(UnkInitParam_wh));
    data_02080f1c->childInit[idx].aid = (u8)aid;
    MBi_CommChangeParentState(aid, 10, &req.info);
}

void MBi_CommParentRecvDataPerChild(const u8 *data, u16 aid)
{
    UnkPacket_0201a770 pkt;
    u32 state;
    UnkJoinReq_02015f14 *src;
    u8 k;
    u8 type;

    if (aid == 0) {
        return;
    }
    if (aid > 15) {
        return;
    }
    src = MBi_SetRecvBufferFromChild(data + 0xa, &pkt, aid);
    state = data_02080f1c->unk14e8[aid - 1];
    switch (pkt.type) {
    case 0:
        break;
    case 7:
        if (state == 2) {
            if (src == NULL) {
                return;
            }
            func_02015f14_Req(aid, aid - 1, src);
        }
        if (state != 10) {
            return;
        }
        {
            u8 cnt = 0;
            k = src->group;
            if (k >= 16 || ENTRY_02015f14(k).unk5ca == 0 ||
                data_02080f1c->unk14a8[aid - 1] != ENTRY_02015f14(k).info->ggid) {
                data_02080f1c->unk1754[aid - 1] = 4;
            } else {
                u8 j;
                for (j = 0; j < 16; j++) {
                    if ((1 << j) & ENTRY_02015f14(k).unk5c6) {
                        cnt++;
                    }
                }
                if (cnt >= ENTRY_02015f14(k).info->maxEntry) {
                    data_02080f1c->unk1754[aid - 1] = 0;
                    MBi_CommChangeParentState(aid, 11, NULL);
                    return;
                }
            }
        }
        switch (data_02080f1c->unk1754[aid - 1]) {
        case 3:
            func_02015f14_Join(aid, k);
            break;
        case 4:
            data_02080f1c->unk1754[aid - 1] = 0;
            MBi_CommChangeParentState(aid, 4, NULL);
            break;
        }
        break;
    case 8:
        if (state == 5) {
            MBi_CommChangeParentState(aid, 14, NULL);
            return;
        }
        if (state != 14) {
            return;
        }
        if (data_02080f1c->unk1754[aid - 1] != 2) {
            return;
        }
        k = data_02080f1c->unk1526[aid - 1];
        ENTRY_02015f14(k).unk5c4 |= 1 << aid;
        ENTRY_02015f14(k).unk5c0 = 0;
        data_02080f1c->unk1754[aid - 1] = 0;
        MBi_CommChangeParentState(aid, 6, NULL);
        break;
    case 9:
        if (state != 6) {
            return;
        }
        k = data_02080f1c->unk1526[aid - 1];
        if (k == 0xff) {
            return;
        }
        ENTRY_02015f14(k).unk5c2 = MBi_calc_nextsendblock(ENTRY_02015f14(k).unk5c2, pkt.u.h.val);
        break;
    case 10:
        if (state == 6) {
            k = data_02080f1c->unk1526[aid - 1];
            if (k == 0xff) {
                return;
            }
            ENTRY_02015f14(k).unk5c4 &= ~(1 << aid);
            MBi_CommChangeParentState(aid, 7, NULL);
            return;
        }
        if (state != 7) {
            return;
        }
        if (data_02080f1c->unk1754[aid - 1] != 5) {
            return;
        }
        data_02080f1c->unk1754[aid - 1] = 0;
        MBi_CommChangeParentState(aid, 8, NULL);
        break;
    case 11:
        break;
    }
}
