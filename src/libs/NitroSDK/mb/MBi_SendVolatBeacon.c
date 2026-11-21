#include <nitro/types.h>

typedef struct {
    u8 lo : 4;
    u8 aid : 4;
    u8 count;
    u16 data[10];
} UnkMbEntry_02017a18; /* 0x16 */

typedef struct {
    u32 ggid;         /* 0x00 */
    u8 lo : 2;        /* 0x04 */
    u8 hi : 6;
    u8 unk5;          /* 0x05 */
    u8 unk6;          /* 0x06 */
    u8 seq;           /* 0x07 */
    u16 sum;          /* 0x08 */
    u8 unkA;          /* 0x0a */
    u8 padB;
    u16 unkC;         /* 0x0c */
    u16 unkE;         /* 0x0e */
    UnkMbEntry_02017a18 entries[4]; /* 0x10 */
    u8 unk68[8];      /* 0x68 */
} UnkMbBeacon_02017a18;

typedef struct {
    void *unk0;       /* 0x00 */
    u8 *obj;          /* 0x04 */
    u8 *cur;          /* 0x08 */
    u8 state;         /* 0x0c */
    u8 padD;
    u8 unkE;          /* 0x0e */
    u8 unkF;          /* 0x0f */
    u8 unk10;         /* 0x10 */
    u8 unk11;         /* 0x11 */
} UnkMbBeaconCtx_02017a18;

extern UnkMbBeaconCtx_02017a18 data_02080e74;
extern UnkMbBeacon_02017a18 data_02080ea0;
extern u16 data_02080ea8;
extern UnkMbEntry_02017a18 data_02080eb0[];
extern u32 data_02080e6c;
extern void (*data_02080e70)(u32 ggid);

void MBi_InitSendVolatBeacon(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
u16 MBi_calc_cksum(const void *p, u32 size);
s32 WM_SetGameInfo(void *cb, const void *data, u32 size, u32 a, u32 b, u8 c);

void MBi_SendVolatBeacon(u32 a, u32 b, u8 flags)
{
    s32 i;
    u32 n;
    u16 diff;
    if (data_02080e74.unkE != data_02080e74.obj[0x4b4]) {
        MBi_InitSendVolatBeacon();
    }
    {
        u8 *obj = data_02080e74.obj;
        data_02080ea0.lo = 2;
        data_02080ea0.unk5 = obj[0x4b3];
        data_02080ea0.unk6 = data_02080e74.unkE;
        data_02080ea0.ggid = *(u32 *)(obj + 0x4b8);
        data_02080ea0.hi = obj[0x4b5];
        data_02080ea0.seq = data_02080e74.unk11++;
        data_02080ea0.unkA = obj[0x358];
        data_02080ea0.unkC = *(u16 *)(obj + 0x35a);
        data_02080ea0.unkE = *(u16 *)(obj + 0x35c);
        if (data_02080e6c == 0 && data_02080e70 != NULL) {
            data_02080e70(*(u32 *)(obj + 0x4b8));
        }
    }
    for (i = 0; i < 8; i++) {
        data_02080ea0.unk68[i] = data_02080e74.obj[0x4a8 + i];
    }
    {
        vu16 z = 0;
        MIi_CpuClear16(z, data_02080eb0, 0x58);
    }
    n = 0;
    diff = *(u16 *)(data_02080e74.obj + 0x4b0) ^ *(u16 *)(data_02080e74.obj + 0x35a);
    for (i = 0; i < 15; i++) {
        if (diff & (2 << i)) {
            MIi_CpuCopy16(data_02080e74.obj + 0x35e + i * 0x16, &data_02080eb0[n], 0x16);
            n++;
            *(u16 *)(data_02080e74.obj + 0x4b0) |= 2 << i;
            if (n == 4) {
                break;
            }
        }
    }
    if (n < 4) {
        data_02080eb0[n].aid = 0;
    }
    data_02080ea0.sum = 0;
    data_02080ea0.sum = MBi_calc_cksum(&data_02080ea8, 0x68);
    if (*(u16 *)(data_02080e74.obj + 0x4b0) == *(u16 *)(data_02080e74.obj + 0x35a)) {
        data_02080e74.state = 1;
    }
    WM_SetGameInfo(NULL, &data_02080ea0, 0x70, a, b, flags | 2);
    if (data_02080e6c == 1 && data_02080e70 != NULL) {
        data_02080e70(*(u32 *)(data_02080e74.obj + 0x4b8));
    }
}
