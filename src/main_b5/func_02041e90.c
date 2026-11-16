#include <nitro/types.h>

typedef struct {
    u16 val;
    u16 flags;
} UnkStruct_0224ffa0;

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[0xc];
    u16 unk14;
    u8 pad16[2];
    u16 cnt;
    u16 unk1a;
    u32 unk1c;
} UnkWork_02041e90;

extern UnkStruct_0224ffa0 gWirelessSendBuffer;

u16 WH_GetBitmap(void);
u32 func_0203c300(void);
u32 func_02042904(u16 v);
s32 func_0203c340(void);
void WH_SendData(void *p, u32 size, void (*fn)(void));
void WH_SetReceiver(void (*fn)(void));
void WFS_EnableSync(u16 v);
void DC_StoreRange(void *p, u32 size);
void func_02042364(void);
void func_02042384(void);
void func_02041454(void);
void func_02041e54(void);

void func_02041e90(UnkWork_02041e90 *w) {
    UnkStruct_0224ffa0 *p;
    u16 v;

    switch (w->mode) {
    case 1:
        v = WH_GetBitmap();
        break;
    case 2:
        v = func_0203c300();
        break;
    }
    if (func_02042904(w->unk14) != func_02042904(v))
        return;
    p = &gWirelessSendBuffer;
    switch (w->mode) {
    case 1:
        p->val = v;
        p->flags |= 2;
        w->cnt = 5;
        w->unk1a = 0;
        w->fn = func_02041454;
        break;
    case 2:
        if (func_0203c340() != 2)
            return;
        WH_SetReceiver(func_02042364);
        p->val = v;
        p->flags = 1;
        DC_StoreRange(p, 4);
        WH_SendData(p, 4, func_02042384);
        WFS_EnableSync(v);
        w->unk1a = 0;
        w->fn = func_02041e54;
        break;
    }
}
