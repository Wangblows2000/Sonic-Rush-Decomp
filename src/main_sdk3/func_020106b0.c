#include <nitro/types.h>

typedef struct UnkWmCb_020106b0 {
    u16 apiid;     /* 0x00 */
    u16 errcode;   /* 0x02 */
    u16 unk4;      /* 0x04 */
    u16 port;      /* 0x06 */
    u32 unk8;      /* 0x08 */
    u32 unkC;      /* 0x0c */
    u16 unk10;     /* 0x10 */
    u16 unk12;     /* 0x12 */
    u8 pad14[6];
    u16 unk1a;     /* 0x1a */
    void (*cb)(struct UnkWmCb_020106b0 *); /* 0x1c */
    u16 unk20;     /* 0x20 */
} UnkWmCb_020106b0;

typedef void (*UnkWmFunc_020106b0)(UnkWmCb_020106b0 *);

typedef struct {
    u32 unk0;
    u8 *status;          /* 0x04 */
    u8 pad8[8];
    UnkWmCb_020106b0 *fifoBuf; /* 0x10 */
    u8 pad14[2];
    u16 unk16;           /* 0x16 */
    UnkWmFunc_020106b0 cb[41]; /* 0x18 */
    UnkWmFunc_020106b0 indCb;  /* 0xbc */
    UnkWmFunc_020106b0 portCb[16]; /* 0xc0 */
    void *portArg[16];   /* 0x100 */
} UnkWmSys_020106b0;

extern UnkWmSys_020106b0 *data_0207faec;
extern u16 data_0207fae8;
extern UnkWmCb_020106b0 data_0207fb2c;
extern u8 data_0207fb40[];
extern u8 data_0207fb50[];

void DC_InvalidateRange(void *p, u32 size);
void DC_StoreRange(void *p, u32 size);
void OS_Terminate(void);
void func_02010cec(void);
void func_02010694(void);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

void func_020106b0(u32 tag, UnkWmCb_020106b0 *data, BOOL err)
{
    if (err) {
        return;
    }
    DC_InvalidateRange(data_0207faec->fifoBuf, 0x100);
    if (data_0207faec->unk16 == 0) {
        DC_InvalidateRange(data_0207faec->status, 0x800);
    }
    if (data != data_0207faec->fifoBuf) {
        DC_InvalidateRange(data, 0x100);
    }
    if (data->apiid >= 0x29) {
        if (data->apiid == 0x80) {
            if (data->errcode == 0x13) {
                OS_Terminate();
            }
            if (data_0207faec->indCb != NULL) {
                data_0207faec->indCb(data);
            }
        } else if (data->apiid == 0x82) {
            if (data_0207faec->portCb[data->port] != NULL) {
                data->cb = data_0207faec->portArg[data->port];
                DC_InvalidateRange((void *)data->unk8, *(u16 *)(data_0207faec->status + 0x46));
                data_0207faec->portCb[data->port](data);
            }
        } else if (data->apiid == 0x81) {
            data->apiid = 0xf;
            if (data->cb != NULL) {
                data->cb(data);
            }
        }
    } else {
        if (data->apiid == 0xe && (data->unk4 == 11 || data->unk4 == 12) && data->errcode == 0) {
            DC_InvalidateRange((void *)data->unk8, *(u16 *)(data_0207faec->status + 0x46));
        }
        if (data->apiid == 2 && data->errcode == 0) {
            UnkWmFunc_020106b0 cb = data_0207faec->cb[data->apiid];
            func_02010cec();
            if (cb != NULL) {
                cb(data);
            }
            return;
        }
        if (data_0207faec->cb[data->apiid] != NULL) {
            data_0207faec->cb[data->apiid](data);
            if (data_0207fae8 == 0) {
                return;
            }
        }
        if (data->apiid == 8 || data->apiid == 0xc) {
            u16 i;
            u16 state;
            u16 a;
            u16 b;
            u8 *mac;
            u16 *ssid;
            if (data->apiid == 8) {
                mac = (u8 *)data + 0xa;
                ssid = (u16 *)((u8 *)data + 0x14);
                state = *(u16 *)((u8 *)data + 8);
                a = data->unk10;
                b = 0;
            } else if (data->apiid == 0xc) {
                a = 0;
                state = *(u16 *)((u8 *)data + 8);
                b = data->unk10;
                ssid = NULL;
                mac = (u8 *)data + 0xa;
            }
            if (state == 7 || state == 9) {
                data_0207fb2c.apiid = 0x82;
                data_0207fb2c.errcode = 0;
                data_0207fb2c.unk4 = state;
                data_0207fb2c.unk8 = 0;
                data_0207fb2c.unkC = 0;
                data_0207fb2c.unk10 = 0;
                data_0207fb2c.unk12 = a;
                data_0207fb2c.unk20 = b;
                data_0207fb2c.unk1a = 0xffff;
                MI_CpuCopy8(mac, data_0207fb40, 6);
                if (ssid != NULL) {
                    MIi_CpuCopy16(ssid, data_0207fb50, 0x18);
                } else {
                    vu16 z = 0;
                    MIi_CpuClear16(z, data_0207fb50, 0x18);
                }
                for (i = 0; i < 16; i++) {
                    data_0207fb2c.port = i;
                    if (data_0207faec->portCb[i] != NULL) {
                        data_0207fb2c.cb = data_0207faec->portArg[i];
                        data_0207faec->portCb[i](&data_0207fb2c);
                    }
                }
            }
        }
    }
    DC_InvalidateRange(data_0207faec->fifoBuf, 0x100);
    func_02010694();
    if (data != data_0207faec->fifoBuf) {
        data->apiid |= 0x8000;
        DC_StoreRange(data, 0x100);
    }
}
